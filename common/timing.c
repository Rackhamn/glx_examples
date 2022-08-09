#include "timing.h"

/* need more features for convertion between time */
/* minutes to seconds to milliseconds to microseconds to nanoseconds */
/* both with integers and with floats */

long int get_time_ns(void) {
	long int time;
	struct timespec ts;
	
	clock_gettime(CLOCK_MONOTONIC, &ts);
	
	time = (long int)(ts.tv_sec * 1000000000) + (long int)(ts.tv_nsec);
	return time;
}

unsigned long get_time_us() {
	unsigned long time;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	
	time = ts.tv_nsec / 1000;
	time += ts.tv_sec * 1000000;
	
	return time;
}

unsigned long get_time_ms() {
	unsigned long time;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	
	time = ts.tv_nsec / 1000000;
	time += ts.tv_sec * 1000;
	
	return time;
}

/* start&end is in NS, wait is in Milli Seconds */
/* time is not good enough */
void sleep_ms(long int start_time, long int end_time, long int max_sleep_time_ms) {
	long int elapsed_time, sleep_time;
	struct timespec req;
	
	elapsed_time = end_time - start_time;
	
	sleep_time = max_sleep_time_ms * 1000000; /* ms to ns */
	if(elapsed_time < sleep_time && elapsed_time > 0) {
		sleep_time = sleep_time - elapsed_time;
	
		/* get seconds */
		req.tv_sec = 0;
		while(sleep_time > 1000000000) {
			req.tv_sec++;
			sleep_time -= 1000000000;
		}
		/* get nanoseconds */
		req.tv_nsec = sleep_time;

		clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
	}
}

void sleep_ns(unsigned long int sleep_time_ns) {
	struct timespec req;
	
	/* get seconds */
	req.tv_sec = 0;
	while(sleep_time_ns > 1000000000) {
		req.tv_sec++;
		sleep_time_ns -= 1000000000;
	}
	/* get nanoseconds */
	req.tv_nsec = sleep_time_ns;

	clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
}

void clear_meta_timing(struct meta_timing_t * meta) {
	meta->frame.start = 0;
	meta->frame.end = 0;
	meta->input.start = 0;
	meta->input.end = 0;
	meta->update.start = 0;
	meta->update.end = 0;
	meta->render.start = 0;
	meta->render.end = 0;
	meta->sleep.start = 0;
	meta->sleep.end = 0;
}
