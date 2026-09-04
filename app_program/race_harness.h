/*
 *	poc_minimal.h
 *	Minimal C1 race PoC entry point.
 */

#ifndef POC_MINIMAL_H
#define POC_MINIMAL_H

#include <tk/tkernel.h>

/* Called from usermain (app_main.c) when RUN_RACE_HARNESS=1. */
IMPORT INT usermain_raceharness( void );

#endif /* POC_MINIMAL_H */
