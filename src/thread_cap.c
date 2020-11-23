#include "mesh_utils.h"
#include "mesh_server.h"


void simcom_msg_handler(struct_mesh_msg *operation,struct sockaddr_in* addr)
{
	dbg_time("Recv:ops = %d,%s\n",operation->msg_type,operation->data);

	switch(operation->msg_type)
	{
		case MESH_MSG_TYPE_REMOTE_SET_RE_REQ:
		{
			pthread_cond_signal(&simcom_cond[PTHREAD_TYPE_RE]);
			execute_cmds("/usr/sbin/mesh_config remode");

			sleep(1);
			pthread_cond_wait(&simcom_cond[PTHREAD_TYPE_CAP], &simcom_mutex[PTHREAD_TYPE_CAP]);
			break;
		}

		case MESH_MSG_TYPE_REMOTE_SET_RE_RESP:
		{
			dbg_time("Recv msg from RE: %s\n",operation->data);
			dbg_time("Addr:[%s:%d]",inet_ntoa(addr->sin_addr),(int)ntohs(addr->sin_port));
			break;
		}

		default:
			break;
	}
}


void * thread_cap_run(void *param)
{
	char* mode;

	mode = get_mesh_mode();
	if(is_mesh_cap_mode(mode))
	{
		pthread_mutex_lock(&simcom_mutex[PTHREAD_TYPE_CAP]);
		pthread_cond_wait(&simcom_cond[PTHREAD_TYPE_CAP], &simcom_mutex[PTHREAD_TYPE_CAP]);
		pthread_mutex_unlock (&simcom_mutex[PTHREAD_TYPE_CAP]);
	}

	cap_loop();
	
	pthread_exit(NULL);
}

void cap_loop()
{
	int sockfd;
	struct_mesh_msg operation={0};
	struct sockaddr_in addr,client;
	socklen_t peerlen = sizeof(addr);

	dbg_time("Cap loop\n");

	if((sockfd = socket(PF_INET,SOCK_DGRAM,0))==-1)
	{
		dbg_time("fail to socket\n");
		pthread_exit(NULL);
	}

	memset(&addr,0,sizeof(addr));
	addr.sin_family=PF_INET;
	addr.sin_port=htons(MESH_PROT);
	addr.sin_addr.s_addr=inet_addr(MESH_ADDR);

	if(0 > bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)))
	{
		dbg_time("bind\n");
		dbg_time("Do check br-lan ip address\n");
		pthread_exit(NULL);
	}

	while(1)
	{
		pthread_mutex_lock(&simcom_mutex[PTHREAD_TYPE_CAP]);
		memset(&operation,0,sizeof(operation));
		recvfrom(sockfd,&operation,sizeof(operation),0,(struct sockaddr*)&client,&peerlen);
		sendto(sockfd,&operation,sizeof(operation),0,(struct sockaddr*)&client,peerlen);
		simcom_msg_handler(&operation,&client);
		pthread_mutex_unlock (&simcom_mutex[PTHREAD_TYPE_CAP]);
	}

	close(sockfd);

}


