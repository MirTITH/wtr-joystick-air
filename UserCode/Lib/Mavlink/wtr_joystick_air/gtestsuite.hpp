/** @file
 *	@brief MAVLink comm testsuite protocol generated from wtr_joystick_air.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <gtest/gtest.h>
#include "wtr_joystick_air.hpp"

#ifdef TEST_INTEROP
using namespace mavlink;
#undef MAVLINK_HELPER
#include "mavlink.h"
#endif


TEST(wtr_joystick_air, JOYSTICK_AIR)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR packet_in{};
    packet_in.buttons = {{ 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 }};
    packet_in.switchs = 101;
    packet_in.knobs = {{ 17651, 17652 }};
    packet_in.joysticks = {{ 17.0, 18.0 }};

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR packet1{};
    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.buttons, packet2.buttons);
    EXPECT_EQ(packet1.switchs, packet2.switchs);
    EXPECT_EQ(packet1.knobs, packet2.knobs);
    EXPECT_EQ(packet1.joysticks, packet2.joysticks);
}

#ifdef TEST_INTEROP
TEST(wtr_joystick_air_interop, JOYSTICK_AIR)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_joystick_air_t packet_c {
         { 17.0, 18.0 }, { 17651, 17652 }, { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 }, 101
    };

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR packet_in{};
    packet_in.buttons = {{ 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 }};
    packet_in.switchs = 101;
    packet_in.knobs = {{ 17651, 17652 }};
    packet_in.joysticks = {{ 17.0, 18.0 }};

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR packet2{};

    mavlink_msg_joystick_air_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.buttons, packet2.buttons);
    EXPECT_EQ(packet_in.switchs, packet2.switchs);
    EXPECT_EQ(packet_in.knobs, packet2.knobs);
    EXPECT_EQ(packet_in.joysticks, packet2.joysticks);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(wtr_joystick_air, JOYSTICK_AIR_LED)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_LED packet_in{};
    packet_in.r = 17.0;
    packet_in.g = 45.0;
    packet_in.b = 73.0;
    packet_in.lightness = 101.0;

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_LED packet1{};
    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_LED packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.r, packet2.r);
    EXPECT_EQ(packet1.g, packet2.g);
    EXPECT_EQ(packet1.b, packet2.b);
    EXPECT_EQ(packet1.lightness, packet2.lightness);
}

#ifdef TEST_INTEROP
TEST(wtr_joystick_air_interop, JOYSTICK_AIR_LED)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_joystick_air_led_t packet_c {
         17.0, 45.0, 73.0, 101.0
    };

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_LED packet_in{};
    packet_in.r = 17.0;
    packet_in.g = 45.0;
    packet_in.b = 73.0;
    packet_in.lightness = 101.0;

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_LED packet2{};

    mavlink_msg_joystick_air_led_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.r, packet2.r);
    EXPECT_EQ(packet_in.g, packet2.g);
    EXPECT_EQ(packet_in.b, packet2.b);
    EXPECT_EQ(packet_in.lightness, packet2.lightness);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif

TEST(wtr_joystick_air, JOYSTICK_AIR_Dashboard)
{
    mavlink::mavlink_message_t msg;
    mavlink::MsgMap map1(msg);
    mavlink::MsgMap map2(msg);

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_Dashboard packet_in{};
    packet_in.id = 17;
    packet_in.title = to_char_array("FGHIJKLMNOPQRSTUVWX");
    packet_in.value = 17.0;

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_Dashboard packet1{};
    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_Dashboard packet2{};

    packet1 = packet_in;

    //std::cout << packet1.to_yaml() << std::endl;

    packet1.serialize(map1);

    mavlink::mavlink_finalize_message(&msg, 1, 1, packet1.MIN_LENGTH, packet1.LENGTH, packet1.CRC_EXTRA);

    packet2.deserialize(map2);

    EXPECT_EQ(packet1.id, packet2.id);
    EXPECT_EQ(packet1.title, packet2.title);
    EXPECT_EQ(packet1.value, packet2.value);
}

#ifdef TEST_INTEROP
TEST(wtr_joystick_air_interop, JOYSTICK_AIR_Dashboard)
{
    mavlink_message_t msg;

    // to get nice print
    memset(&msg, 0, sizeof(msg));

    mavlink_joystick_air_dashboard_t packet_c {
         17.0, 17, "FGHIJKLMNOPQRSTUVWX"
    };

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_Dashboard packet_in{};
    packet_in.id = 17;
    packet_in.title = to_char_array("FGHIJKLMNOPQRSTUVWX");
    packet_in.value = 17.0;

    mavlink::wtr_joystick_air::msg::JOYSTICK_AIR_Dashboard packet2{};

    mavlink_msg_joystick_air_dashboard_encode(1, 1, &msg, &packet_c);

    // simulate message-handling callback
    [&packet2](const mavlink_message_t *cmsg) {
        MsgMap map2(cmsg);

        packet2.deserialize(map2);
    } (&msg);

    EXPECT_EQ(packet_in.id, packet2.id);
    EXPECT_EQ(packet_in.title, packet2.title);
    EXPECT_EQ(packet_in.value, packet2.value);

#ifdef PRINT_MSG
    PRINT_MSG(msg);
#endif
}
#endif
