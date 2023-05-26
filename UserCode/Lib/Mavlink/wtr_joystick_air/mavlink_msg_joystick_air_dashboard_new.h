#pragma once
// MESSAGE JOYSTICK_AIR_DASHBOARD_NEW PACKING

#define MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW 211


typedef struct __mavlink_joystick_air_dashboard_new_t {
 uint8_t id; /*<  */
 char title[20]; /*<  */
} mavlink_joystick_air_dashboard_new_t;

#define MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN 21
#define MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN 21
#define MAVLINK_MSG_ID_211_LEN 21
#define MAVLINK_MSG_ID_211_MIN_LEN 21

#define MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC 39
#define MAVLINK_MSG_ID_211_CRC 39

#define MAVLINK_MSG_JOYSTICK_AIR_DASHBOARD_NEW_FIELD_TITLE_LEN 20

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_JOYSTICK_AIR_DASHBOARD_NEW { \
    211, \
    "JOYSTICK_AIR_DASHBOARD_NEW", \
    2, \
    {  { "id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_joystick_air_dashboard_new_t, id) }, \
         { "title", NULL, MAVLINK_TYPE_CHAR, 20, 1, offsetof(mavlink_joystick_air_dashboard_new_t, title) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_JOYSTICK_AIR_DASHBOARD_NEW { \
    "JOYSTICK_AIR_DASHBOARD_NEW", \
    2, \
    {  { "id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_joystick_air_dashboard_new_t, id) }, \
         { "title", NULL, MAVLINK_TYPE_CHAR, 20, 1, offsetof(mavlink_joystick_air_dashboard_new_t, title) }, \
         } \
}
#endif

/**
 * @brief Pack a joystick_air_dashboard_new message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param id  
 * @param title  
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_new_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t id, const char *title)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN];
    _mav_put_uint8_t(buf, 0, id);
    _mav_put_char_array(buf, 1, title, 20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN);
#else
    mavlink_joystick_air_dashboard_new_t packet;
    packet.id = id;
    mav_array_memcpy(packet.title, title, sizeof(char)*20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
}

/**
 * @brief Pack a joystick_air_dashboard_new message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param id  
 * @param title  
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_new_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t id,const char *title)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN];
    _mav_put_uint8_t(buf, 0, id);
    _mav_put_char_array(buf, 1, title, 20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN);
#else
    mavlink_joystick_air_dashboard_new_t packet;
    packet.id = id;
    mav_array_memcpy(packet.title, title, sizeof(char)*20);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
}

/**
 * @brief Encode a joystick_air_dashboard_new struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param joystick_air_dashboard_new C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_new_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_joystick_air_dashboard_new_t* joystick_air_dashboard_new)
{
    return mavlink_msg_joystick_air_dashboard_new_pack(system_id, component_id, msg, joystick_air_dashboard_new->id, joystick_air_dashboard_new->title);
}

/**
 * @brief Encode a joystick_air_dashboard_new struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param joystick_air_dashboard_new C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_new_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_joystick_air_dashboard_new_t* joystick_air_dashboard_new)
{
    return mavlink_msg_joystick_air_dashboard_new_pack_chan(system_id, component_id, chan, msg, joystick_air_dashboard_new->id, joystick_air_dashboard_new->title);
}

/**
 * @brief Send a joystick_air_dashboard_new message
 * @param chan MAVLink channel to send the message
 *
 * @param id  
 * @param title  
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_joystick_air_dashboard_new_send(mavlink_channel_t chan, uint8_t id, const char *title)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN];
    _mav_put_uint8_t(buf, 0, id);
    _mav_put_char_array(buf, 1, title, 20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW, buf, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
#else
    mavlink_joystick_air_dashboard_new_t packet;
    packet.id = id;
    mav_array_memcpy(packet.title, title, sizeof(char)*20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW, (const char *)&packet, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
#endif
}

/**
 * @brief Send a joystick_air_dashboard_new message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_joystick_air_dashboard_new_send_struct(mavlink_channel_t chan, const mavlink_joystick_air_dashboard_new_t* joystick_air_dashboard_new)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_joystick_air_dashboard_new_send(chan, joystick_air_dashboard_new->id, joystick_air_dashboard_new->title);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW, (const char *)joystick_air_dashboard_new, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
#endif
}

#if MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_joystick_air_dashboard_new_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t id, const char *title)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, id);
    _mav_put_char_array(buf, 1, title, 20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW, buf, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
#else
    mavlink_joystick_air_dashboard_new_t *packet = (mavlink_joystick_air_dashboard_new_t *)msgbuf;
    packet->id = id;
    mav_array_memcpy(packet->title, title, sizeof(char)*20);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW, (const char *)packet, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_MIN_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_CRC);
#endif
}
#endif

#endif

// MESSAGE JOYSTICK_AIR_DASHBOARD_NEW UNPACKING


/**
 * @brief Get field id from joystick_air_dashboard_new message
 *
 * @return  
 */
static inline uint8_t mavlink_msg_joystick_air_dashboard_new_get_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field title from joystick_air_dashboard_new message
 *
 * @return  
 */
static inline uint16_t mavlink_msg_joystick_air_dashboard_new_get_title(const mavlink_message_t* msg, char *title)
{
    return _MAV_RETURN_char_array(msg, title, 20,  1);
}

/**
 * @brief Decode a joystick_air_dashboard_new message into a struct
 *
 * @param msg The message to decode
 * @param joystick_air_dashboard_new C-struct to decode the message contents into
 */
static inline void mavlink_msg_joystick_air_dashboard_new_decode(const mavlink_message_t* msg, mavlink_joystick_air_dashboard_new_t* joystick_air_dashboard_new)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    joystick_air_dashboard_new->id = mavlink_msg_joystick_air_dashboard_new_get_id(msg);
    mavlink_msg_joystick_air_dashboard_new_get_title(msg, joystick_air_dashboard_new->title);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN? msg->len : MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN;
        memset(joystick_air_dashboard_new, 0, MAVLINK_MSG_ID_JOYSTICK_AIR_DASHBOARD_NEW_LEN);
    memcpy(joystick_air_dashboard_new, _MAV_PAYLOAD(msg), len);
#endif
}
