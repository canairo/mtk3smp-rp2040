#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <bsp/libbsp.h>

#include "usb_console_compat.h"
#include "race_harness.h"

#define POOL_SIZE	32768
#define BUTTON_PIN	20
#define HEARTBEAT_OPS	100

LOCAL void churn_task(INT stacd, void *exinf)
{
	UW	n = 0, ok = 0, fail = 0;

	(void)exinf;

  // wait for gp20 to be pressed //
	gpio_set_pin(BUTTON_PIN, GPIO_MODE_IN);
	tm_printf((UB *)"[churn%d] waiting for GP20\n", stacd);
	while ( gpio_get_val(BUTTON_PIN) != 0 ) {
		tk_dly_tsk(10);
	}

  tm_printf((UB *)"[RACE COND] program started! \n[RACE COND]it should take around 20k mallocs / frees for the program to suddenly crash.\n", stacd);
	tm_printf((UB *)"[churn%d] race started\n", stacd);

	for(;;) {
		void	*p = Kmalloc(POOL_SIZE);

		if ( p != NULL ) {
			ok++;
			Kfree(p);
		} else {
			fail++;
		}

		n++;
		if ( (n % HEARTBEAT_OPS) == 0 ) {
			tm_printf((UB *)"[churn%d] n=%u ok=%u fail=%u\n",
				  stacd, n, ok, fail);
		}
	}
}

LOCAL T_CTSK ctsk_churn0 = {
	.itskpri	= 10,
	.stksz		= 4096,
	.task		= churn_task,
	.tskatr		= TA_HLNG | TA_RNG3 | TA_ASSPRC,
	.assprc		= TP_PRC1,
};

LOCAL T_CTSK ctsk_churn1 = {
	.itskpri	= 10,
	.stksz		= 4096,
	.task		= churn_task,
	.tskatr		= TA_HLNG | TA_RNG3 | TA_ASSPRC,
	.assprc		= TP_PRC2,
};

EXPORT INT usermain_raceharness(void)
{
	ID	tid;

	tid = tk_cre_tsk(&ctsk_churn0);
	if ( tid > E_OK ) tk_sta_tsk(tid, 0);
	tid = tk_cre_tsk(&ctsk_churn1);
	if ( tid > E_OK ) tk_sta_tsk(tid, 1);

	tk_slp_tsk(TMO_FEVR);
	return 0;
}
