#include "FreeRTOS.h"

void __malloc_lock(struct _reent* r)
{
	(void)r;
	vPortEnterCritical();
}

void __malloc_unlock(struct _reent* r)
{
	(void)r;
	vPortExitCritical();
}
