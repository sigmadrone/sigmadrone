 /*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */
#include <stdint.h>
#include <stdio.h>
#include <stdexcept>
#include "digitalout.h"
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "colibrihwmap.h"
#include "usbstoragedevice.h"

extern "C"
{
#include "usbd_msc.h"
#include "usbd_core.h"
extern USBD_DescriptorsTypeDef MSC_Desc;
extern USBD_StorageTypeDef USBD_DISK_fops;
}

static USBD_HandleTypeDef USBD_Device;
static DigitalOut usb_otg_fs1_pso(USBOTG_FS1_PSO_PIN);

void UsbStorageDevice::start()
{
#ifdef USE_CORTEX_USB_OTG
	usb_otg_fs1_pso.write(1);
	/* Init MSC Application */
	USBD_Init(&USBD_Device, &MSC_Desc, 0);

	USBD_MSC_PreInit(&USBD_Device);

	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, USBD_MSC_CLASS);

	/* Add Storage callbacks for MSC Class */
	USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops);

	/* Start Device Process */
	USBD_Start(&USBD_Device);
#endif
}

