
#ifndef MESH_SERVER_H
#define MESH_SERVER_H


#define MESH_ADDR "127.0.0.1"
#define MESH_PROT 7777

#define RE_LOOP_TO 20

#define MESSAGE_DATA_CONFIG_RE_REQ "Remote config RE"
#define MESSAGE_DATA_CONFIG_RE_RESP_PASS "Config RE pass!"

typedef enum{
	MESH_MSG_TYPE_REMOTE_SET_RE_REQ,
	MESH_MSG_TYPE_REMOTE_SET_RE_RESP,
	MESH_MSG_TYPE_MAX
}mesh_msg_type_enum;

typedef enum PTHREAD_TYPE
{
	PTHREAD_TYPE_CAP,
	PTHREAD_TYPE_RE,
	PTHREAD_TYPE_NUM
}PTHREAD_TYPE_T;


typedef struct {
	mesh_msg_type_enum msg_type;
	char data[40];
} struct_mesh_msg;


static pthread_t simcom_tid[PTHREAD_TYPE_NUM] = {0};
static pthread_mutex_t simcom_mutex[PTHREAD_TYPE_NUM];
static pthread_cond_t simcom_cond[PTHREAD_TYPE_NUM] = {0};

void * thread_cap_run(void *param);
void * thread_re_run(void *param);
void cap_loop();
void re_loop();
int mesh_send_message(mesh_msg_type_enum msg_type, char* ip_address, int mesh_port);
#endif
