#!/usr/bin/env bash
# Anduril / FSM build wrapper script
# Copyright (C) 2017-2023 Selene ToyKeeper
# SPDX-License-Identifier: GPL-3.0-or-later

# Usage: build-all.sh [pattern]
# If pattern given, only build targets which match.

# enable "**" for recursive glob (requires bash)
shopt -s globstar

if [ "${DEBUG}" == "1" ]; then
  set -x
  export DEBUG
fi

POSITIONAL=()
for arg in "${@}"
do
    if [[ "${arg}" == "--user" ]]
    then
        shift # past arg
        USER_CFG_DIRNAME="${1}"
        USER_CFG_DIR="users/${1}"
	USER_CFG=1
        echo "Loading user config from 'users/${1}'" >&2
    elif [[ "${arg}" =~ --no-user ]]
    then
	unset USER_CONF USER_CFG_DIR USER_CFG_DIRNAME
        export SKIP_USER_CFG=1
    elif [[ "${arg}" == "--user-cfg" ]] || [[ "${arg}" == "--user-config" ]]
    then
        shift # past arg
        export USER_CFG_DIR="${1}"
	USER_CFG=1
    elif [[ "${arg}" == "--debug" ]]
    then
        set -x
        export DEBUG=1
    else
        # Store var in a temporary array to restore after we've finished parsing for builder-specific args
        POSITIONAL+=("$1")
    fi
    shift
done
set -- "${POSITIONAL[@]}" # restore positional parameters

function main {
    if [ "$#" -gt 0 ]; then
        # multiple search terms with "AND"
        SEARCH=( "$@" )
        # memes
        [ "$1" = "me" ] && shift && shift && echo "Make your own $*." && exit 1
    fi

    # TODO: detect UI from $0 and/or $*
    UI=anduril

    [ ! -d hex/ ] && mkdir -p hex

    make-version-h  # generate a version.h file

    PASS=0
    FAIL=0
    PASSED=''
    FAILED=''

    # Get a default user config to use, if there is one and it isn't already overridden
    if [ -f user.cfg ] && [ -z "${SKIP_USER_CFG}" ]
    then
        #TODO: Allow building user and base versions at the same time
        USER_CFG_DIRNAME="$(<user.cfg)" && echo "Loaded default user config: ${USER_CFG_DIRNAME} from user.cfg. To skip user config, use --no-user" >&2
        if [ -d "users/${USER_CFG_DIRNAME}" ]
        then
            export USER_CFG_DIR="users/${USER_CFG_DIRNAME}"
        fi
        unset USER_CFG_DIRNAME
    fi

    # load user global config, if any. Check directory users/username if --user was specified
    if [ "$USER_CFG" == 1 ]
    then
        if [ -d "${USER_CFG_DIR}" ]
        then
            export USER_CFG_DIR="users/${USER_NAME}"
        else
	    echo "User config directory not found."
	    exit;
	fi
    fi

    # build targets are hw/$vendor/$model/**/$ui.h
    for TARGET in hw/*/*/**/"$UI".h ; do

        # friendly name for this build
        NAME=$(echo "$TARGET" | perl -ne 's|/|-|g; /hw-(.*)-'"$UI"'.h/ && print "$1\n";')

        # Get a default user config to use, if there is one and it isn't already overridden
        if [ -f user.cfg ] && [ -z "${SKIP_USER_CFG}" ]
        then
            export USER_CFG_DIRNAME="$(<user.cfg)"
            if [ -d "users/${USER_CFG_DIRNAME}" ]
            then
                export USER_CFG_DIR="users/${USER_CFG_DIRNAME}"
	        #echo "Loaded user config dir ${USER_CFG_DIR} from user.cfg" >&2
            fi
        fi

        export USER_DEFAULT_CFG="${USER_CFG_DIR}/${UI}.h"
        # TODO: allow multiple custom model builds per user
        export USER_MODEL_CFG="${USER_CFG_DIR}/model/${NAME}/${UI}.h"

        # limit builds to searched patterns, if given
        SKIP=0
        if [ ${#SEARCH[@]} -gt 0 ]; then
            for text in "${SEARCH[@]}" ; do
                if ! echo "$NAME $TARGET" | grep -i -- "$text" > /dev/null ; then
                    SKIP=1
                fi
            done
        fi
        if [ 1 = $SKIP ]; then continue ; fi

        # announce what we're going to build
        echo "===== $UI $REV : $NAME ====="

        # try to compile, track result, and rename compiled files
        if bin/build.sh "$TARGET" ; then
            if [ ! -z "${USER_CFG_DIRNAME}" ]
            then
                HEX_USERPART="_${USER_CFG_DIRNAME}"
            else
                HEX_USERPART="" # noop
            fi
            HEX_OUT="hex/$UI.${NAME}${HEX_USERPART}.hex"
            mv -f "ui/$UI/$UI".hex "$HEX_OUT"
            MD5=$(md5sum "$HEX_OUT" | cut -d ' ' -f 1)
            echo "  # $MD5"
            echo "  > $HEX_OUT"
            PASS=$((PASS + 1))
            PASSED="$PASSED $NAME"
        else
            echo "ERROR: build failed"
            FAIL=$((FAIL + 1))
            FAILED="$FAILED $NAME"
        fi

    done

    # summary
    echo "===== $PASS builds succeeded, $FAIL failed ====="
    #echo "PASS: $PASSED"
    if [ 0 != $FAIL ]; then
        echo "FAIL:$FAILED"
        exit 1
    fi
}

function make-version-h {
    # old: version = build date
    #date '+#define VERSION_NUMBER "%Y-%m-%d"' > ui/$UI/version.h

    REV=$(bin/version-string.sh c)
    # save the version name to version.h
    mkdir -p ".build/$UI"
    echo '#define VERSION_NUMBER "'"$REV"'"' > ".build/$UI/version.h"
}

main "$@"

