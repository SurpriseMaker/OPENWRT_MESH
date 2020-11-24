
#ifndef MESH_SERVER_H
#define MESH_SERVER_H

#define MESH_PROT 7777

#define MESSAGE_DATA_CONFIG_RE_REQ "Remote config RE"
#define MESSAGE_DATA_CONFIG_RE_RESP_PASS "Config RE pass!"

typedef enum{
	MESH_MSG_TYPE_REMOTE_SET_RE_REQ,
	MESH_MSG_TYPE_REMOTE_SET_RE_RESP,
	MESH_MSG_TYPE_MAX
}mesh_msg_type_enum;


typedef struct {
	mesh_msg_type_enum msg_type;
	char data[40];
} struct_mesh_msg;


static pthread_t mesh_tid = 0;

void * mesh_server_run(void *param);
void server_loop();

#endif
