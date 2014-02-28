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
 * @file        portal.h
 * @brief       CeBIT 2014 demo application - portal node
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#ifndef __PORTAL_H
#define __PORTAL_H


void portal_in(int argc, char** argv);

void portal_out(int src, int type, int data);


#endif /* __PORTAL_H */
