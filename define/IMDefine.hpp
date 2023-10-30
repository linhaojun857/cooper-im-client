#ifndef config_IMCONFIG_HPP_
#define config_IMCONFIG_HPP_

#define APP_TCP_SERVER_IP ("127.0.0.1")
#define APP_TCP_SERVER_PORT (8888)
#define PING_TYPE 100
#define PONG_TYPE 200

#define HTTP_SERVER_URL "http://127.0.0.1:9999"
#define HTTP_SUCCESS_CODE (20000)
#define HTTP_ERROR_CODE (30000)

#define DEFAULT_USER_STATUS_ONLINE ("在线")
#define DEFAULT_USER_STATUS_OFFLINE ("离线")

#define PROTOCOL_TYPE_BASE (1000)
#define PROTOCOL_TYPE_AUTH_MSG (PROTOCOL_TYPE_BASE + 1)
#define PROTOCOL_TYPE_ERROR_MSG (PROTOCOL_TYPE_BASE + 2)
#define PROTOCOL_TYPE_FRIEND_APPLY_NOTIFY_I (PROTOCOL_TYPE_BASE + 3)
#define PROTOCOL_TYPE_FRIEND_APPLY_NOTIFY_P (PROTOCOL_TYPE_BASE + 4)
#define PROTOCOL_TYPE_FRIEND_ENTITY (PROTOCOL_TYPE_BASE + 5)
#define PROTOCOL_TYPE_SYNC_COMPLETE_MESSAGE (PROTOCOL_TYPE_BASE + 6)
#define PROTOCOL_TYPE_PERSON_MESSAGE_SEND (PROTOCOL_TYPE_BASE + 7)
#define PROTOCOL_TYPE_PERSON_MESSAGE_RECV (PROTOCOL_TYPE_BASE + 8)

#define SYNC_DATA_FRIEND_ENTITY_INSERT 1
#define SYNC_DATA_FRIEND_ENTITY_UPDATE 2
#define SYNC_DATA_FRIEND_ENTITY_DELETE 3

#define SYNC_DATA_PERSON_MESSAGE_INSERT 1
#define SYNC_DATA_PERSON_MESSAGE_DELETE 2

#define MSG_TYPE_TEXT 0
#define MSG_TYPE_IMAGE 1
#define MSG_TYPE_ViDEO 2
#define MSG_TYPE_FILE 3

#endif
