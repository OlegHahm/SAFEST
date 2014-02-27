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
 * @file        light.h
 * @brief       RGB-LED actuator node
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef __LIGHT_H
#define __LIGHT_H


void light_init(void);

void light_set_shell(int argc, char **argv);

void light_off_shell(int argc, char **argv);

void light_set_rgb(uint8_t r, uint8_t g, uint8_t b);

void light_alarm(void);

void light_ok(void);

void light_warn(void);

void light_off(void);


#endif /* __LIGHT_H */
/** @} */