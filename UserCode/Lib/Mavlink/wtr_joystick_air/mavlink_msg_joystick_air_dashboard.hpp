// MESSAGE JOYSTICK_AIR_Dashboard support class

#pragma once

namespace mavlink {
namespace wtr_joystick_air {
namespace msg {

/**
 * @brief JOYSTICK_AIR_Dashboard message
 *
 * Show message and values on remote controller
 */
struct JOYSTICK_AIR_Dashboard : mavlink::Message {
    static constexpr msgid_t MSG_ID = 211;
    static constexpr size_t LENGTH = 25;
    static constexpr size_t MIN_LENGTH = 25;
    static constexpr uint8_t CRC_EXTRA = 34;
    static constexpr auto NAME = "JOYSTICK_AIR_Dashboard";


    uint8_t id; /*<   */
    std::array<char, 20> title; /*<   */
    float value; /*<    */


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
        ss << "  id: " << +id << std::endl;
        ss << "  title: \"" << to_string(title) << "\"" << std::endl;
        ss << "  value: " << value << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << value;                         // offset: 0
        map << id;                            // offset: 4
        map << title;                         // offset: 5
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> value;                         // offset: 0
        map >> id;                            // offset: 4
        map >> title;                         // offset: 5
    }
};

} // namespace msg
} // namespace wtr_joystick_air
} // namespace mavlink
