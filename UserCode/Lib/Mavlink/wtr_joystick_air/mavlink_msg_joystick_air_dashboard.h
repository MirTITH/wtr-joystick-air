#pragma once
// MESSAGE JOYSTICK_AIR_Dashboard PACKING

#define MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard 211


typedef struct __mavlink_joystick_air_dashboard_t {
 float value; /*<   */
 uint8_t id; /*<  */
 char title[20]; /*<  */
} mavlink_joystick_air_dashboard_t;

#define MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN 25
#define MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN 25
#define MAVLINK_MSG_ID_211_LEN 25
#define MAVLINK_MSG_ID_211_MIN_LEN 25

#define MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC 34
#define MAVLINK_MSG_ID_211_CRC 34

#define MAVLINK_MSG_JOYSTICK_AIR_Dashboard_FIELD_TITLE_LEN 20

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_JOYSTICK_AIR_Dashboard { \
    211, \
    "JOYSTICK_AIR_Dashboard", \
    3, \
    {  { "id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_joystick_air_dashboard_t, id) }, \
         { "title", NULL, MAVLINK_TYPE_CHAR, 20, 5, offsetof(mavlink_joystick_air_dashboard_t, title) }, \
         { "value", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_joystick_air_dashboard_t, value) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_JOYSTICK_AIR_Dashboard { \
    "JOYSTICK_AIR_Dashboard", \
    3, \
    {  { "id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_joystick_air_dashboard_t, id) }, \
         { "title", NULL, MAVLINK_TYPE_CHAR, 20, 5, offsetof(mavlink_joystick_air_dashboard_t, title) }, \
         { "value", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_joystick_air_dashboard_t, value) }, \
         } \
}
#endif

/**
 * @brief Pack a joystick_air_dashboard message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param id  
 * @param title  
 * @param value   
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t id, const char *title, float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN];
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, id);
    _mav_put_char_array(buf, 5, title, 20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN);
#else
    mavlink_joystick_air_dashboard_t packet;
    packet.value = value;
    packet.id = id;
    mav_array_memcpy(packet.title, title, sizeof(char)*20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
}

/**
 * @brief Pack a joystick_air_dashboard message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param id  
 * @param title  
 * @param value   
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t id,const char *title,float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN];
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, id);
    _mav_put_char_array(buf, 5, title, 20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN);
#else
    mavlink_joystick_air_dashboard_t packet;
    packet.value = value;
    packet.id = id;
    mav_array_memcpy(packet.title, title, sizeof(char)*20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
}

/**
 * @brief Encode a joystick_air_dashboard struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param joystick_air_dashboard C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_joystick_air_dashboard_t* joystick_air_dashboard)
{
    return mavlink_msg_joystick_air_dashboard_pack(system_id, component_id, msg, joystick_air_dashboard->id, joystick_air_dashboard->title, joystick_air_dashboard->value);
}

/**
 * @brief Encode a joystick_air_dashboard struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param joystick_air_dashboard C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_joystick_air_dashboard_t* joystick_air_dashboard)
{
    return mavlink_msg_joystick_air_dashboard_pack_chan(system_id, component_id, chan, msg, joystick_air_dashboard->id, joystick_air_dashboard->title, joystick_air_dashboard->value);
}

/**
 * @brief Send a joystick_air_dashboard message
 * @param chan MAVLink channel to send the message
 *
 * @param id  
 * @param title  
 * @param value   
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_joystick_air_dashboard_send(mavlink_channel_t chan, uint8_t id, const char *title, float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN];
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, id);
    _mav_put_char_array(buf, 5, title, 20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard, buf, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
#else
    mavlink_joystick_air_dashboard_t packet;
    packet.value = value;
    packet.id = id;
    mav_array_memcpy(packet.title, title, sizeof(char)*20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard, (const char *)&packet, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
#endif
}

/**
 * @brief Send a joystick_air_dashboard message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_joystick_air_dashboard_send_struct(mavlink_channel_t chan, const mavlink_joystick_air_dashboard_t* joystick_air_dashboard)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_joystick_air_dashboard_send(chan, joystick_air_dashboard->id, joystick_air_dashboard->title, joystick_air_dashboard->value);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard, (const char *)joystick_air_dashboard, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
#endif
}

#if MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_joystick_air_dashboard_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t id, const char *title, float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, id);
    _mav_put_char_array(buf, 5, title, 20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard, buf, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
#else
    mavlink_joystick_air_dashboard_t *packet = (mavlink_joystick_air_dashboard_t *)msgbuf;
    packet->value = value;
    packet->id = id;
    mav_array_memcpy(packet->title, title, sizeof(char)*20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard, (const char *)packet, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_CRC);
#endif
}
#endif

#endif

// MESSAGE JOYSTICK_AIR_Dashboard UNPACKING


/**
 * @brief Get field id from joystick_air_dashboard message
 *
 * @return  
 */
static inline uint8_t mavlink_msg_joystick_air_dashboard_get_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field title from joystick_air_dashboard message
 *
 * @return  
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_get_title(const mavlink_message_t* msg, char *title)
{
    return _MAV_RETURN_char_array(msg, title, 20,  5);
}

/**
 * @brief Get field value from joystick_air_dashboard message
 *
 * @return   
 */
static inline float mavlink_msg_joystick_air_dashboard_get_value(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Decode a joystick_air_dashboard message into a struct
 *
 * @param msg The message to decode
 * @param joystick_air_dashboard C-struct to decode the message contents into
 */
static inline void mavlink_msg_joystick_air_dashboard_decode(const mavlink_message_t* msg, mavlink_joystick_air_dashboard_t* joystick_air_dashboard)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    joystick_air_dashboard->value = mavlink_msg_joystick_air_dashboard_get_value(msg);
    joystick_air_dashboard->id = mavlink_msg_joystick_air_dashboard_get_id(msg);
    mavlink_msg_joystick_air_dashboard_get_title(msg, joystick_air_dashboard->title);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN? msg->len : MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN;
        memset(joystick_air_dashboard, 0, MAVLINK_MSG_ID_JOYSTICK_AIR_Dashboard_LEN);
    memcpy(joystick_air_dashboard, _MAV_PAYLOAD(msg), len);
#endif
}
