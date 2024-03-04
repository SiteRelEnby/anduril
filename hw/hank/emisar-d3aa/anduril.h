// Emisar D3aa config options for Anduril
// Copyright (C) 2023 thefreeman, Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#define HWDEF_H hank/emisar-d3aa/hwdef.h

// HPRsense : 4.2+0.3+20 = 24.5mR
// Vsense=42.46mV, R1= 165k
// LPRsense : 3R3
// HDR ratio: 131.5
// transition DAC level 20, ramp level 48
// fifth power ramp 0.02mA to 2001mA

#define RAMP_SIZE 150

// 4 ramp segments:
//   - low  1.024V
//   - low  2.5  V
//   - high 1.024V
//   - high 2.5  V
// HDR ratio: 131.5
// PWM1: DAC Data
// (level_calc.py 4.3287 1 150 7135 5 0.01 1400 --pwm 400000)
// (top level for each "gear": 30 40 120 150)
#define PWM1_LEVELS \
           3,4,5,7,9,12,15,19,24,29,35,43,51,61,72,84,99,115,133,153,176,202,230,262,297,335,377,424,475,531,592,658,730,809,894,986, \
         444,488,535,586,641,700,763,830,903,980,1023, \
          20,22,24,26,28,30,32,35,37,40,43,46,49,52,56,60,63,68,72,77,81,86,92,97,103,109,115,122,129,136,143,151,159,168,177,186,196,206,216,227,239,250,263,275,289,302,316,331,346,362,379,396,413,432,450,470,490,511,532,555,578,602,626,651,678,705,733,761,791,821,853,885,919,953,989, \
         420,435,451,467,484,501,519,537,556,575,595,615,636,657,679,702,725,749,773,798,824,850,877,905,933,962,992,1023
#define PWM2_LEVELS \
         V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10, \
         V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25, \
         V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10,V10, \
         V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25,V25
#define MAX_1x7135            47
#define HDR_ENABLE_LEVEL_MIN  48
#define DEFAULT_LEVEL         50

// no PWM, so MCU clock speed can be slow
#define HALFSPEED_LEVEL      41
#define QUARTERSPEED_LEVEL   40  // seems to run fine at 10kHz/4, try reducing more

#define RAMP_SMOOTH_FLOOR    1
#define RAMP_SMOOTH_CEIL     119  // 35% / 0.7A / 700 lm
// 1 22 [44] 65 87 108 130
#define RAMP_DISCRETE_FLOOR  1
#define RAMP_DISCRETE_CEIL   119
#define RAMP_DISCRETE_STEPS  7

// 20 [45] 70 95 120
#define SIMPLE_UI_FLOOR      20
#define SIMPLE_UI_CEIL       129  // 50% / ~1A / ~860 lm
#define SIMPLE_UI_STEPS      5

// don't blink mid-ramp
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

// thermal config

// temperature limit
#define THERM_FASTER_LEVEL  129 // stop panicking at 50%/1A
#define MIN_THERM_STEPDOWN  MAX_1x7135


// UI

#define SIMPLE_UI_ACTIVE 0 // advanced UI by default, because prototype

// allow Aux Config and Strobe Modes in Simple UI
//#define USE_EXTENDED_SIMPLE_UI

// Allow 3C in Simple UI for switching between smooth and stepped ramping
#define USE_SIMPLE_UI_RAMPING_TOGGLE

#define DEFAULT_2C_STYLE 1 // enable 2 click turbo


// AUX

#define USE_BUTTON_LED

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS

// show each channel while it scroll by in the menu
#define USE_CONFIG_COLORS

// blink numbers on the main LEDs by default
#define DEFAULT_BLINK_CHANNEL  CM_MAIN

// use aux red + aux blue for police strobe
#define USE_POLICE_COLOR_STROBE_MODE
#define POLICE_STROBE_USES_AUX
#define POLICE_COLOR_STROBE_CH1        CM_AUXRED
#define POLICE_COLOR_STROBE_CH2        CM_AUXBLU

// the aux LEDs are front-facing, so turn them off while main LEDs are on
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif


// Misc

#define PARTY_STROBE_ONTIME 1  // slow down party strobe
#define STROBE_OFF_LEVEL 1  // keep the regulator chip on between pulses

// smoother candle mode with bigger oscillations
#define CANDLE_AMPLITUDE 40

// enable 13H factory reset so it can be used on tail e-switch lights
#define USE_SOFT_FACTORY_RESET

// TODO: disable lowpass while asleep; the MCU oversamples

