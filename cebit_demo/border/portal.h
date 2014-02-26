/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @brief       Pseudo border router implementation based on RIOTs shell.
 * 
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef __PORTAL_H
#define __PORTAL_H



void portal_in(int argc, char** argv);

void portal_out(int src, int type, int data);


#endif /* __PORTAL_H */