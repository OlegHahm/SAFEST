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
 * @brief       RGB-LED actuator node - node configuration
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define NODE_ADDRESS            "17";
#define RADIO_CHANNEL           3;


#define LIGHT_COLOR_OK          {0, 255, 0}
#define LIGHT_COLOR_WARN        {255, 255, 0}
#define LIGHT_COLOR_ALARM       {255, 0, 0}

#define LIGHT_CH_R              1
#define LIGHT_CH_G              2
#define LIGHT_CH_B              0



#endif /* __CONFIG_H */
 /** @} */