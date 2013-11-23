#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <kernel.h>
#include <flags.h>
#include <thread.h>
#include <hwtimer.h>
#include <board.h>
#include <smb380-board.h>

#include "sense.h"
#include "destiny.h"
#include "demo.h"


#define SENSE_STACK_SIZE		(KERNEL_CONF_STACKSIZE_DEFAULT + KERNEL_CONF_STACKSIZE_PRINTF)
#define SAMPLING_INTERVAL		5			// interval to sample data in ms
#define ACC_SENSITIVITY			256			// number of stops per G
#define EVENT_LOCK_TICKS		3000		// 3000 * 5ms = 15s before an event can be resent

// define default data threshholds for the event detection
#define DEFAULT_THRESHOLD_1		(270U)		// threshhold for uncritical event
#define DEFAULT_THRESHOLD_2		(270U)		// threshhold for critical event
#define DEFAULT_COUNTER_1		(20U)		// number of thresshold crossings until non-crit event is detected
#define DEFAULT_COUNTER_2		(40U)		// number of high threshold crossings until critical event is detected
#define DEFAULT_TIMEOUT			(50)		// number of samping intervals until counters are reset


// event loop thread memory and thread id
char sense_stack[SENSE_STACK_SIZE];
int sense_tid;

// sense loop status
int do_sense = 0;
int interval = SAMPLING_INTERVAL;

// acceleration data
int16_t acc_data[6];
float acc_norm[3];

// detection fsm data
void (*fsm_state)(void);
int fsm_conf_t1;
int fsm_conf_t2;
int fsm_conf_c1;
int fsm_conf_c2;
int fsm_conf_to;
int fsm_time = 0;
int fsm_c1 = 0;
int fsm_c2 = 0;

// lock event reporting for some time
int lock_evt = 0;
int lock_crit = 0;
int lock_counter = 0;


/**
 * Forward function definitions
 */
int16_t math_modulus(int16_t *v, int dim);
void fsm_init(int t1, int t2, int c1, int c2, int to);
void fsm_reset(void);
void fsm_process(void);
void fsm_eval(void);
void evt_noncrit(void);
void evt_crit(void);

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
			fsm_process();
			if (lock_counter > 0) {
				--lock_counter;
			} else {
				lock_evt = 0;
				lock_crit = 0;
			}
		}
		hwtimer_wait(HWTIMER_TICKS(1000 * interval));
		LED_RED_TOGGLE;
	}
}


void sense_init(char *unsued)
{
	fsm_init(DEFAULT_THRESHOLD_1, DEFAULT_THRESHOLD_2, DEFAULT_COUNTER_1, DEFAULT_COUNTER_2, DEFAULT_TIMEOUT);
	SMB380_init_simple(100, SMB380_BAND_WIDTH_375HZ, SMB380_RANGE_2G);
	puts("SENSE: Acceleration sensor initialized");
	sense_tid = thread_create(sense_stack, SENSE_STACK_SIZE, PRIORITY_MAIN - 1, CREATE_WOUT_YIELD | CREATE_STACKTEST, sense_thread, "sense");
	if (sense_tid > 0) {
		puts("SENSE: Succesfully created thread");
	}
	LED_GREEN_ON;
}

void sense_enable(char *unused)
{
	do_sense = 1;
}

void sense_disable(char *unused)
{
	do_sense = 0;
}

void sense_setparam(char *str)
{
	char *part;
	int t1, t2, c1, c2, to, ival;
	printf("input: %c\n", str);
	int n = sscanf(str, "sense_set %u %u %u %u %u %u", &t1, &t2, &c1, &c2, &to, &ival);
	if (n < 6) {
		puts("Wrong parameters, usage:");
		puts("sense_set threshold_l threshold_h counter_l counter_h timeout interval");
	} else {
		fsm_init(t1, t2, c1, c2, to);
		interval = ival;
		printf("Set paremeters, t1: %i, t2: %i, c1: %i, c2: %i, to: %i, int: %i\n", t1, t2, c1, c2, to, ival);
	}
}

void fsm_init(int t1, int t2, int c1, int c2, int to)
{
	fsm_conf_t1 = t1;
	fsm_conf_t2 = t2;
	fsm_conf_c1 = c1;
	fsm_conf_c2 = c2;
	fsm_conf_to = to;
	fsm_reset();
}

void fsm_reset(void)
{
	fsm_c1 = 0;
	fsm_c2 = 0;
	fsm_time = fsm_conf_to;
}

void fsm_process(void)
{
	int16_t mod = math_modulus(acc_data, 3);
	--fsm_time;
	// detect threshhold crossings
	if (mod > fsm_conf_t1) {
		++fsm_c1;
		if (mod > fsm_conf_t2) {
			++fsm_c2;
		}
	}
	// detect events
	if (fsm_time == 0) {
		fsm_eval();
		fsm_reset();
	}
}

void fsm_eval(void)
{
	printf("EVAL: crit: %i; non-crit: %i\n", fsm_c2, fsm_c1);
	if (fsm_c2 > fsm_conf_c2) {
		evt_crit();
	} else if (fsm_c1 > fsm_conf_c1) {
		evt_noncrit();
	}
}

void evt_noncrit(void)
{
	if (lock_evt == 0) {
		udp_send("send 16 evt");
		puts("----------- EVENT: non-critical ----------");
		LED_GREEN_TOGGLE;
		lock_counter = EVENT_LOCK_TICKS;
		lock_evt = 1;
	}
}

void evt_crit(void)
{
	if (lock_crit == 0) {
		udp_send("send 16 crit");
		puts("########### EVENT: critical ##############");
		LED_RED_TOGGLE;
		lock_counter = EVENT_LOCK_TICKS;
		lock_evt = 1;
		lock_crit = 1;
	}
}

int16_t math_modulus(int16_t *v, int dim)
{
	float mod = 0.0f;
	for (int i = 0; i < dim; i++) {
		mod += powf(v[i], 2.0f);
	}
	return (int16_t)sqrtf(mod);
}
