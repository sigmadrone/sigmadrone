//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// Do not include on semihosting and when freestanding
#if !defined(OS_USE_SEMIHOSTING) && !(__STDC_HOSTED__ == 0)

// ----------------------------------------------------------------------------

#include <errno.h>
#include "diag/Trace.h"
#include "stm32f4xx.h"

// ----------------------------------------------------------------------------

// When using retargetted configurations, the standard write() system call,
// after a long way inside newlib, finally calls this implementation function.

// Based on the file descriptor, it can send arrays of characters to
// different physical devices.

// Currently only the output and error file descriptors are tested,
// and the characters are forwarded to the trace device, mainly
// for demonstration purposes. Adjust it for your specific needs.

// For freestanding applications this file is not used and can be safely
// ignored.

ssize_t
_write (int fd, const char* buf, size_t nbyte);

static void usart_put_char(USART_TypeDef *usart, unsigned char ch)
{
	/* Put character on the serial line */
	usart->DR = (ch & (uint16_t)0x01FF);

	/* Loop until transmit data register is empty */
	while ((usart->SR & USART_FLAG_TXE) == (uint16_t) RESET);
}

ssize_t usart_write(USART_TypeDef *usart, const char* buf __attribute__((unused)), size_t nbyte __attribute__((unused)))
{
	size_t i;
	for (i = 0; i < nbyte; i++) {
		char ch = buf[i];
		if (ch == '\n' && (i + 1) < nbyte && buf[i + 1] == '\r') {
			usart_put_char(usart, ch);
			ch = buf[++i];
			usart_put_char(usart, ch);
		} else if (ch == '\r' && (i + 1) < nbyte && buf[i + 1] == '\n') {
			usart_put_char(usart, ch);
			ch = buf[++i];
			usart_put_char(usart, ch);
		} else if (ch == '\n') {
			usart_put_char(usart, ch);
			usart_put_char(usart, '\r');
		} else {
			usart_put_char(usart, ch);
		}
	}
	return nbyte;
}

ssize_t
_write (int fd __attribute__((unused)), const char* buf __attribute__((unused)),
	size_t nbyte __attribute__((unused)))
{
	if (fd == 1 || fd == 2)
	{
		usart_write(USART6, buf, nbyte);
	}
#if defined(TRACE)
  // STDOUT and STDERR are routed to the trace device
  if (fd == 1 || fd == 2)
    {
      return trace_write (buf, nbyte);
    }
#endif // TRACE

  errno = ENOSYS;
  return -1;
}

// ----------------------------------------------------------------------------

#endif // !defined(OS_USE_SEMIHOSTING) && !(__STDC_HOSTED__ == 0)
