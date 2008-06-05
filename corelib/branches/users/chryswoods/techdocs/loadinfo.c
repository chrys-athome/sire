/*
 * A simple program to calculate load information for single CPU linux machine
 * Written by Ori Idan Helicon technologies Ltd.
 *
 * This program is a Free software licensed under the GPL
 * see: http://www.gnu.org/licenses/licenses.html#GPL
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* 
 * Theory of operation:
 * The Linux kernel stores CPU times in /proc/stat
 * The first line is CPU times given in four times:
 * 1. user time - time spent in user space
 * 2. nice time - time spent in niced tasks (tasks with positive nice value)
 * 3. system time - time spent in kernel space
 * 4. idle time - time spent in idle task, doing nothing
 *
 * This program takes all four times and calculate the percentage of time
 * spent in user, nice and system together, this gives a usage pecentage.
 * Calculation is done for one second.
 */

#define USER 0
#define NICE 1
#define SYSTEM 2
#define IDLE 3

/* used by getopt */
extern int optind;
extern char *optarg;

unsigned int *GetTimes(void) {
	static unsigned int times[4];
	char tmp[5];
	char str[80];
	FILE *file;

	file = fopen("/proc/stat", "r");
	fgets(str, 80, file);
	sscanf(str, "%s %u %u %u %u", tmp, &times[0], &times[1], &times[2], &times[3]);
	fclose(file);
	return times;
}

unsigned int *DiffTimes(void) {
	static unsigned int times1[4];
	unsigned int *times;
	int i;

	memcpy(times1, GetTimes(), sizeof(unsigned int) * 4);
	sleep(1);
	times = GetTimes();

	for(i = 0; i < 4; i++) 
		times1[i] = times[i] - times1[i];
	return times1;
}

int main(void) {
	unsigned int total;
	double load;
	int i;
	unsigned int *times;
	
	printf("loadinfo ver. 0.1 Print CPU load info\n");
	printf("Copyright (c) Ori Idan Helicon technologies Ltd. 2005\n");
	
	times = DiffTimes();
	total = 0;
	for(i = 0; i < 4; i++)
		total += times[i];

	load = (double)times[IDLE] * 100.0 / (double)total;
	printf("CPU load: %2.2f\n", 100.0 - load);
}


