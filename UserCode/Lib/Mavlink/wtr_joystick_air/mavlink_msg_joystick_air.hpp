// MESSAGE JOYSTICK_AIR support class

#pragma once

namespace mavlink {
namespace wtr_joystick_air {
namespace msg {

/**
 * @brief JOYSTICK_AIR message
 *
 *  Components on remote controller
 */
struct JOYSTICK_AIR : mavlink::Message {
    static constexpr msgid_t MSG_ID = 209;
    static constexpr size_t LENGTH = 33;
    static constexpr size_t MIN_LENGTH = 33;
    static constexpr uint8_t CRC_EXTRA = 172;
    static constexpr auto NAME = "JOYSTICK_AIR";


    std::array<uint8_t, 20> buttons; /*<   Buttons  */
    uint8_t switchs; /*<   bit0: left sw. bit1: right sw  */
    std::array<int16_t, 2> knobs; /*<   [0]: left knob. [1]: right knob  */
    std::array<float, 2> joysticks; /*<   [0]: left joystick. [1]: right joystick  */


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
        ss << "  buttons: [" << to_string(buttons) << "]" << std::endl;
        ss << "  switchs: " << +switchs << std::endl;
        ss << "  knobs: [" << to_string(knobs) << "]" << std::endl;
        ss << "  joysticks: [" << to_string(joysticks) << "]" << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << joysticks;                     // offset: 0
        map << knobs;                         // offset: 8
        map << buttons;                       // offset: 12
        map << switchs;                       // offset: 32
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> joysticks;                     // offset: 0
        map >> knobs;                         // offset: 8
        map >> buttons;                       // offset: 12
        map >> switchs;                       // offset: 32
    }
};

} // namespace msg
} // namespace wtr_joystick_air
} // namespace mavlink
