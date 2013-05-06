#define STEPPERS_NUM_AXES 5
enum steppers_modes {
	ST_MODE_STOP = 0,
	ST_MODE_CUBIC,
	ST_MODE_CONST_V,
	ST_MODE_CONST_A
};

struct steppers_target {
	int mode[STEPPERS_NUM_AXES];
	double dt;
	double x[STEPPERS_NUM_AXES];
	double v[STEPPERS_NUM_AXES];
};

void steppersReset(void);
void steppersStatus(void);
void steppersMainLoop(void);
int steppersQueue(struct steppers_target target);
