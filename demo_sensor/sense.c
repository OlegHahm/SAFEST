#include <stdio.h>
#include <stdint.h>

#include <kernel.h>
#include <flags.h>
#include <thread.h>
#include <hwtimer.h>
#include <board.h>
#include <smb380-board.h>

#include "sense.h"


#define SENSE_STACK_SIZE	(KERNEL_CONF_STACKSIZE_DEFAULT + KERNEL_CONF_STACKSIZE_PRINTF)
#define SAMPLING_INTERVAL	25		// interval to sample data in ms


// event loop thread memory and thread id
char sense_stack[SENSE_STACK_SIZE];
int sense_tid;

// sense loop status
int do_sense = 0;

// acceleration data
int16_t acc_data[6];

void print_acc_data(void)
{
	printf("Acc x,y,z: %6d %6d %6d\n", acc_data[SMB380_X_AXIS], acc_data[SMB380_Y_AXIS], acc_data[SMB380_Z_AXIS]);
}

void sense_thread(void) 
{
	for (;;) {
		if (do_sense) {
			SMB380_getAcceleration(SMB380_X_AXIS, &acc_data[SMB380_X_AXIS], &acc_data[SMB380_X_AXIS + 3]);
			SMB380_getAcceleration(SMB380_Y_AXIS, &acc_data[SMB380_Y_AXIS], &acc_data[SMB380_Y_AXIS + 3]);
			SMB380_getAcceleration(SMB380_Z_AXIS, &acc_data[SMB380_Z_AXIS], &acc_data[SMB380_Z_AXIS + 3]);
			print_acc_data();
		}
		hwtimer_wait(HWTIMER_TICKS(1000 * SAMPLING_INTERVAL));
		LED_RED_TOGGLE;
	}
}


void sense_init(void)
{
	SMB380_init_simple(100, SMB380_BAND_WIDTH_100HZ, SMB380_RANGE_2G);
	puts("SENSE: Acceleration sensor initialized");
	sense_tid = thread_create(sense_stack, SENSE_STACK_SIZE, PRIORITY_MAIN - 1, CREATE_WOUT_YIELD | CREATE_STACKTEST, sense_thread, "sense");
	if (sense_tid > 0) {
		puts("SENSE: Succesfully created thread");
	}
	LED_GREEN_ON;
}

void sense_enable(void)
{
	do_sense = 1;
}

void sense_disable(void)
{
	do_sense = 0;
}