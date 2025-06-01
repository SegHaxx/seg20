#ifdef __GNUC__ // shut up gcc
#define SHL static __attribute__((__unused__))
#else
#define __attribute__() /**/
#define SHL static
#endif

#if !defined(__DJGPP__) && !defined(__MINT__) && !defined(__WATCOMC__) && \
		!defined(__CC65__) && !defined(__Z88DK__) // unix?
	#if 1
	#include <time.h>
	SHL long time_msec(void){
		struct timespec tv;
		clock_gettime(CLOCK_MONOTONIC_COARSE,&tv);
		return (((long)tv.tv_sec)*1000)+(tv.tv_nsec/1000000);
	}
	#else
	#include <sys/time.h>
	SHL time_msec(void){
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
	}
	#endif
#endif

#ifdef __DJGPP__
SHL long long time_msec(){return uclock();}
#endif

#ifdef __MINT__ // Atari ST TOS
#include <osbind.h>
#include <mint/sysvars.h>
SHL long get_hz_200(void){return *(_hz_200);}
SHL long time_msec(void){return 20*Supexec(get_hz_200);}
#endif

#if defined(__CC65__) || defined(__Z88DK__)
#ifndef __APPLE2__
#include <time.h>	
SHL long time_msec(void){return clock()*(1000/CLOCKS_PER_SEC);}
#else
SHL long time_msec(void){return 0;}
#endif
#endif
