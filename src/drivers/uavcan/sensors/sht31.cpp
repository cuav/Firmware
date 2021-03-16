/****************************************************************************
 *
 *   Copyright (c) 2021 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "sht31.hpp"
#include <drivers/drv_hrt.h>
#include <lib/ecl/geo/geo.h>
#include <parameters/param.h>
#include <systemlib/err.h>


const char *const UavcanSht31Bridge::NAME = "sht31_sensor";

UavcanSht31Bridge::UavcanSht31Bridge(uavcan::INode &node) :
	UavcanSensorBridgeBase("uavcan_sht31_sensor", ORB_ID(sht31)),
	_sub_sht31(node)
{
}


int UavcanSht31Bridge::init()
{


	int res = _sub_sht31.start(Sht31CbBinder(this, &UavcanSht31Bridge::sht31_sub_cb));

	if (res < 0) {
		DEVICE_LOG("failed to start uavcan sub: %d", res);
		return res;
	}

	return 0;
}

void UavcanSht31Bridge::sht31_sub_cb(const
				     uavcan::ReceivedDataStructure<ardupilot::Sht31>
				     &msg)
{


	float humidity = msg.humidity;
	float temperature_c = msg.temperature + CONSTANTS_ABSOLUTE_NULL_CELSIUS;

	printf("msg.humidity = %f\n", (double)msg.humidity);
	printf("msg.temperature = %f\n", (double)msg.temperature);

	printf("humidity = %f\n", (double)humidity);
	printf("temperature_c = %f\n", (double)temperature_c);

	sht31_s report = {
		.timestamp = hrt_absolute_time(),
		.temperature = temperature_c,
		.humidity  = humidity,
	};



	publish(msg.getSrcNodeID().get(), &report);
}

