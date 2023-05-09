// MESSAGE JOYSTICK_AIR_LED support class

#pragma once

namespace mavlink {
namespace wtr_joystick_air {
namespace msg {

/**
 * @brief JOYSTICK_AIR_LED message
 *
 * RGB led on remote controller
 */
struct JOYSTICK_AIR_LED : mavlink::Message {
    static constexpr msgid_t MSG_ID = 210;
    static constexpr size_t LENGTH = 16;
    static constexpr size_t MIN_LENGTH = 16;
    static constexpr uint8_t CRC_EXTRA = 82;
    static constexpr auto NAME = "JOYSTICK_AIR_LED";


    float r; /*<   range: [0, 1]  */
    float g; /*<   range: [0, 1]  */
    float b; /*<   range: [0, 1]  */
    float lightness; /*<   Gain of lightness. Must be positive values  */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  r: " << r << std::endl;
        ss << "  g: " << g << std::endl;
        ss << "  b: " << b << std::endl;
        ss << "  lightness: " << lightness << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << r;                             // offset: 0
        map << g;                             // offset: 4
        map << b;                             // offset: 8
        map << lightness;                     // offset: 12
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> r;                             // offset: 0
        map >> g;                             // offset: 4
        map >> b;                             // offset: 8
        map >> lightness;                     // offset: 12
    }
};

} // namespace msg
} // namespace wtr_joystick_air
} // namespace mavlink
