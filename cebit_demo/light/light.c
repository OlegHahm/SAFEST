/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cebit_demo
 * @{
 *
 * @file        light.c
 * @brief       RGB-LED actuator node
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdint.h>
#include <stdio.h>

#include "rgbled.h"
#include "periph/pwm.h"

#include "light.h"
#include "config.h"


static rgbled_t led;


void light_init(void)
{
    rgbled_init(&led, PWM_0, LIGHT_CH_R, LIGHT_CH_G, LIGHT_CH_B);
    rgb_t col = {255, 0, 0};
    rgbled_set(&led, &col);
}

void light_set_shell(int argc, char **argv)
{
    rgb_t col;
    if (argc >= 4) {
        col.r = (uint8_t)atoi(argv[1]);
        col.g = (uint8_t)atoi(argv[2]);
        col.b = (uint8_t)atoi(argv[3]);
        printf("setting color to r: %i, g: %i, b: %i\n", col.r, col.g, col.b);
        rgbled_set(&led, &col);
    }
}

void light_off_shell(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    light_off();
}

void light_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    rgb_t col = {r, g, b};
    rgbled_set(&led, &col);
}

void light_alarm(void)
{
    rgb_t col = LIGHT_COLOR_ALARM;
    rgbled_set(&led, &col);
}

void light_ok(void)
{
    rgb_t col = LIGHT_COLOR_OK;
    rgbled_set(&led, &col);

}

void light_warn(void)
{
    rgb_t col = LIGHT_COLOR_WARN;
    rgbled_set(&led, &col);

}

void light_off(void)
{
    rgb_t col = {0, 0, 0};
    rgbled_set(&led, &col);
}