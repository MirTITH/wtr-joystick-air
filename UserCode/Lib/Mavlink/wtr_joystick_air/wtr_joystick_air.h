/** @file
 *  @brief MAVLink comm protocol generated from wtr_joystick_air.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_WTR_JOYSTICK_AIR_H
#define MAVLINK_WTR_JOYSTICK_AIR_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_WTR_JOYSTICK_AIR.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#define MAVLINK_WTR_JOYSTICK_AIR_XML_HASH -1563113552340428843

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{209, 172, 33, 33, 0, 0, 0}, {210, 82, 16, 16, 0, 0, 0}, {211, 34, 25, 25, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_WTR_JOYSTICK_AIR

// ENUM DEFINITIONS



// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 3
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 3
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_joystick_air.h"
#include "./mavlink_msg_joystick_air_led.h"
#include "./mavlink_msg_joystick_air_dashboard.h"

// base include



#if MAVLINK_WTR_JOYSTICK_AIR_XML_HASH == MAVLINK_PRIMARY_XML_HASH
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_JOYSTICK_AIR, MAVLINK_MESSAGE_INFO_JOYSTICK_AIR_LED, MAVLINK_MESSAGE_INFO_JOYSTICK_AIR_Dashboard}
# define MAVLINK_MESSAGE_NAMES {{ "JOYSTICK_AIR", 209 }, { "JOYSTICK_AIR_Dashboard", 211 }, { "JOYSTICK_AIR_LED", 210 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_WTR_JOYSTICK_AIR_H
