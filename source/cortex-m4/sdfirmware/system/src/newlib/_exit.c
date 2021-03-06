//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdlib.h>
#include "diag/Trace.h"
#include "cortexm/emergency_shutdown.h"

// ----------------------------------------------------------------------------

extern void
__attribute__((noreturn))
__reset_hardware(void);

// ----------------------------------------------------------------------------

// Forward declaration

void
_exit(int code);

// ----------------------------------------------------------------------------

// On Release, call the hardware reset procedure.
// On Debug we just enter an infinite loop, to be used as landmark when halting
// the debugger.
//
// It can be redefined in the application, if more functionality
// is required.

void
__attribute__((weak))
_exit(int code __attribute__((unused)))
{
#if !defined(DEBUG)
  __reset_hardware();
#endif

  EmergencyShutdown_Handler();
  // TODO: write on trace
  while (1)
    ;
}

// ----------------------------------------------------------------------------
#if defined(DEBUG)
#define __DEBUG_BKPT()  asm volatile ("bkpt 0")
#else
#define __DEBUG_BKPT() do {} while(0)
#endif

void
__attribute__((weak,noreturn))
abort(void)
{
  trace_puts("abort(), exiting...");
  __DEBUG_BKPT();
  _exit(1);
}

// ----------------------------------------------------------------------------
