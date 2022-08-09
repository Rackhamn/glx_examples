#ifndef TIMING_H
#define TIMING_H

#include <stdio.h>
#include <unistd.h> // using depricated usleep

#include <sys/types.h>
#ifndef _GNU_SOURCE
	#include <linux/time.h>
#else
	#include <time.h>
#endif


int clock_gettime (clockid_t __clock_id, struct timespec *__tp);
int clock_nanosleep (clockid_t __clock_id, int __flags, const struct timespec *__req, struct timespec *__rem);

long int get_time_ns(void);
unsigned long get_time_us();
unsigned long get_time_ms();

void sleep_ms(long int start_time, long int end_time, long int max_sleep_time_ms);

void sleep_ns(unsigned long int sleep_time_ns);

struct timing_t {
	unsigned long int start, end;
};

struct meta_timing_t {
	/* 5*2 = 10 long int */
	struct timing_t frame, input, update, render, sleep;
};

void clear_meta_timing(struct meta_timing_t * meta);

#endif /* TIMING_H */
