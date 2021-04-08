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

#ifndef SHT31_HPP
#define SHT31_HPP

#include <uORB/topics/sht31.h>


class MavlinkStreamSHT31 : public MavlinkStream
{
public:

	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamSHT31(mavlink); }

	static constexpr const char *get_name_static() { return "SHT31_STATUS"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_SHT31_OUTPUT_STATUS; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return MAVLINK_MSG_ID_SHT31_OUTPUT_STATUS_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
	}

private:
	explicit MavlinkStreamSHT31(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription _sht31_sub{ORB_ID(sht31)};

	/* do not allow top copying this class */
	MavlinkStreamSHT31(MavlinkStreamSHT31 &) = delete;
	MavlinkStreamSHT31 &operator = (const MavlinkStreamSHT31 &) = delete;

	bool send() override
	{
		sht31_s sht31{};
		mavlink_sht31_output_status_t msg;
		_sht31_sub.copy(&sht31);
		msg.sht31_temp = sht31.temperature;
		msg.sht31_humidity = sht31.humidity;

		mavlink_msg_sht31_output_status_send_struct(_mavlink->get_channel(), &msg);

		return true;
	}
};


#endif
