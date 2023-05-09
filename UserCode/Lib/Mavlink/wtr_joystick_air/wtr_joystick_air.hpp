/** @file
 *	@brief MAVLink comm protocol generated from wtr_joystick_air.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <array>
#include <cstdint>
#include <sstream>

#ifndef MAVLINK_STX
#define MAVLINK_STX 253
#endif

#include "../message.hpp"

namespace mavlink {
namespace wtr_joystick_air {

/**
 * Array of msg_entry needed for @p mavlink_parse_char() (through @p mavlink_get_msg_entry())
 */
constexpr std::array<mavlink_msg_entry_t, 3> MESSAGE_ENTRIES {{ {209, 172, 33, 33, 0, 0, 0}, {210, 82, 16, 16, 0, 0, 0}, {211, 34, 25, 25, 0, 0, 0} }};

//! MAVLINK VERSION
constexpr auto MAVLINK_VERSION = 3;


// ENUM DEFINITIONS




} // namespace wtr_joystick_air
} // namespace mavlink

// MESSAGE DEFINITIONS
#include "./mavlink_msg_joystick_air.hpp"
#include "./mavlink_msg_joystick_air_led.hpp"
#include "./mavlink_msg_joystick_air_dashboard.hpp"

// base include

