#define STEPPERS_NUM_AXES 5

struct steppers_target {
	double dt;
	double x[STEPPERS_NUM_AXES];
	double v[STEPPERS_NUM_AXES];
};

void steppersReset(void);
void steppersStatus(void);
void steppersMainLoop(void);
int steppersQueue(struct steppers_target target);
