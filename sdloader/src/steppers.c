#include "xparameters.h"
#include "stdio.h"
#include "xenv.h"

#include "xutil.h"
#include "xstatus.h"
#include <sys/types.h>
#include <inttypes.h>

#include "motion.h"
#include "steppers.h"

#define printf xil_printf

enum {
	STATE_OFF = 0,
	STATE_IDLE,
	STATE_ACTIVE,
	STATE_DELAY,
	STATE_STOPPING
} states;

static int state = STATE_OFF;
static struct steppers_target current_position;

#define STEP_FREQ 100000000
#define STEP_BIT 40
#define ACC_FREQ 250

#define QUEUE_LEN 10
static struct steppers_target queue[QUEUE_LEN];
static int queue_len = 0;

void unqueue_move(void) {
	double dx, v0, vn, dt, ka, kb, a, j;
	int i;
	struct steppers_target next_position = queue[0];
	char buf[500];

	for (i = 0; i < queue_len - 1; i++) {
		queue[i] = queue[i+1];
	};
	queue_len--;

	dx = next_position.x[0] - current_position.x[0];
	v0 = current_position.v[0];
	vn = next_position.v[0];
	dt = next_position.dt;

	kb = 3 * dx / (dt * dt) - (2 * v0 + vn) / dt;
	ka = (vn - v0) / (3.0 * (dt * dt)) - (2.0 * kb) / (3.0 * dt);
	sprintf(buf, "dx: %f v0: %f vn: %f dt: %f ka: %f kb: %f\r\n", dx, v0, vn, dt, ka, kb);
	printf("%s", buf);

	/* v = v0 / 100000000.0 * (double)(1LL<<32); */
	a = 2.0 * kb * ((double)(1LL<<STEP_BIT)) / (double)STEP_FREQ / (double)ACC_FREQ;
	j = 6.0 * ka * ((double)(1LL<<STEP_BIT)) / (double)STEP_FREQ / (double)ACC_FREQ / (double)ACC_FREQ;

	dt = dt * (double)ACC_FREQ;

	printf("MOVE: dt: %ld a: %ld j: %ld\r\n", (uint32_t)dt, (int32_t)a, (int32_t)j);
	MOTION_SetSteps(XPAR_MOTION_X_BASEADDR, (uint32_t)dt); /* 50 ms */
	MOTION_SetA(XPAR_MOTION_X_BASEADDR, (int32_t)a);
	MOTION_SetJ(XPAR_MOTION_X_BASEADDR, (int32_t)j);
	MOTION_Start(XPAR_MOTION_X_BASEADDR);

	current_position = next_position;
}

void unqueue_stop(void) {
	printf("STOP\r\n");
	MOTION_SetSteps(XPAR_MOTION_X_BASEADDR, ACC_FREQ/10); /* 100 ms */
	MOTION_SetV(XPAR_MOTION_X_BASEADDR, 0);
	MOTION_SetA(XPAR_MOTION_X_BASEADDR, 0);
	MOTION_SetJ(XPAR_MOTION_X_BASEADDR, 0);
	MOTION_Start(XPAR_MOTION_X_BASEADDR); /* 10 seconds */
	current_position.v[0] = 0.0;
}

void steppersMainLoop(void) {
	switch (state) {
		case STATE_OFF:
			if (queue_len > 0) {
				printf("INIT\r\n");
				MOTION_mWriteReg(XPAR_MOTION_X_BASEADDR, MOTION_PRE_N_OFFSET,100);
				MOTION_mWriteReg(XPAR_MOTION_X_BASEADDR, MOTION_PULSE_N_OFFSET,300);
				MOTION_mWriteReg(XPAR_MOTION_X_BASEADDR, MOTION_POST_N_OFFSET,100);
				MOTION_mWriteReg(XPAR_MOTION_X_BASEADDR, MOTION_STEP_BIT_OFFSET, (1<<31) | STEP_BIT);
				MOTION_SetDT(XPAR_MOTION_X_BASEADDR, STEP_FREQ/ACC_FREQ);
				current_position.x[0] = 0.0;
				current_position.v[0] = 0.0;
				state = STATE_IDLE;
			}
			break;
		case STATE_IDLE:
			if (queue_len > 0) {
				unqueue_move();
				state = STATE_ACTIVE;
			};
			break;
		case STATE_ACTIVE:
			{
				uint32_t tmp = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_CMD_OFFSET);
				if ((tmp & 1) == 0) {
					if (queue_len > 0) {
						unqueue_move();
					} else {
						tmp = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_STATUS_OFFSET);
						if ((tmp & 1) == 1) {
							printf("done\r\n");
							unqueue_stop();
							state = STATE_STOPPING;
						};
					};
				};
			};
			break;
		case STATE_STOPPING:
			{
				uint32_t tmp = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_CMD_OFFSET);
				if ((tmp & 1) == 0) {
					/* accepted */
					tmp = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_STATUS_OFFSET);
					if ((tmp & 1) == 1) {
						state = STATE_IDLE;
						printf("IDLE\r\n");
					};
				};
			};
			break;
	}
}
	
int steppersQueue(struct steppers_target target) {
	if (queue_len >= QUEUE_LEN)
		return -1;

	printf("add to queue\r\n");
	queue[queue_len] = target;
	queue_len++;
}

	
void steppersReset(void) {
	queue_len = 0;
	state = STATE_OFF;
	current_position.x[0] = 0.0;
	current_position.v[0] = 0.0;
	MOTION_mReset(XPAR_MOTION_X_BASEADDR);
}


void steppersStatus(void)
{
	uint32_t val;

	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_CMD_OFFSET);
	printf("CMD: %08lx\r\n", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_STATUS_OFFSET);
	printf("STATUS: %08lx\r\n", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_DT_OFFSET);
	printf("DT: %10ld    ", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_STEPS_OFFSET);
	printf("STEPS: %7ld\r\n", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_XL_OFFSET);
	printf("XL: %08lx\r\n", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_XH_OFFSET);
	printf("X:  %10ld    ", ((int32_t)val) >> (STEP_BIT-32));
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_V_OFFSET);
	printf("V:  %10ld\r\n", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_A_OFFSET);
	printf("A:  %10ld    ", val);
	val = MOTION_mReadReg(XPAR_MOTION_X_BASEADDR, MOTION_STAT_J_OFFSET);
	printf("J:  %10ld\r\n", val);
}


