
#ifndef MESH_SERVER_H
#define MESH_SERVER_H

#define MESH_PROT 9658
#define MAX_CONNECT_ATTEMPT_TIMES 20

#define MESSAGE_DATA_BYTES 256
#define MESSAGE_DATA_CONFIG_RE_REQ "Remote config RE"
#define MESSAGE_DATA_CONFIG_RE_RESP_PASS "Config RE pass!"
#define MESSAGE_DATA_CONFIG_RE_RESP_UNKNOW "Unknow command."

typedef enum{
	MESH_MSG_TYPE_REMOTE_SET_RE_REQ=95,	
	MESH_MSG_TYPE_REMOTE_SET_RE_RESP,

	MESH_MSG_TYPE_REMOTE_RESTORE_RE_REQ,
	MESH_MSG_TYPE_REMOTE_RESTORE_RE_RESP,
	MESH_MSG_TYPE_MAX
}mesh_msg_type_enum;


typedef struct {
	mesh_msg_type_enum msg_type;
	char data[MESSAGE_DATA_BYTES];
	char verifydata[MESSAGE_DATA_BYTES];
} struct_mesh_msg;

	
void * mesh_server_run(void *param);
void server_loop();

#endif
