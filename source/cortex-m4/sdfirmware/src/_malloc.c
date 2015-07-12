#include "FreeRTOS.h"

void __malloc_lock(struct _reent* r)
{
	vPortEnterCritical();
}

void __malloc_unlock(struct _reent* r)
{
	vPortExitCritical();
}
