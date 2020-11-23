
/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement a thread acting as RE to simulate remote config.

Author: Mr.Tsao Bo

Version: initial

Date: 2020-10-29

*****************************************************************************/

#include "mesh_utils.h"
#include "mesh_config_details.h"
#include "mesh_server.h"


void re_loop()
{
	int sockfd,recv_len,result;
	struct sockaddr_in server_addr;
	
	int client_sock;
	struct sockaddr_in client_addr;
	int clientaddr_size;
	
	socklen_t peerlen = sizeof(server_addr);
	struct_mesh_msg operation={0};
	struct timeval tv_out;
	fd_set readfds;
	fd_set errorfds;
	int status;

	dbg_time("RE loop\n");
	printf("RE loop\n");

	if((sockfd = socket(PF_INET,SOCK_STREAM,0))==-1)
	{
		perror("fail to create socket");
		pthread_exit(NULL);
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(MESH_PROT);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//inet_addr(MESH_ADDR);
        if ((result = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
		printf("[%s] bind : %d\n", __FUNCTION__, result);
            close(sockfd);
            pthread_exit(NULL);
        }
		
	if(listen(sockfd,10)==-1)
	{
		perror("listen failed");
		printf("listen failed");
		pthread_exit(NULL);
	}

	while(1)
	{
		printf("re server is running.\n");
		client_sock = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&clientaddr_size);
        	if (client_sock < 0)
        	{
            		perror("accept failed");
			printf("accept failed");
           	 	pthread_exit(NULL);
        	}
		
			memset(&operation,0,sizeof(operation));
			recv_len = recvfrom(client_sock,&operation,sizeof(operation),0,(struct sockaddr*)&client_addr,&peerlen);
			printf("Rcv msg from CAP:recv_len=%d, operation.date=%s\n",recv_len,operation.data);
			dbg_time("Rcv msg from CAP:recv_len=%d, operation.date=%s\n",recv_len,operation.data);

			memset(&operation,0,sizeof(operation));
			switch(operation.msg_type)
			{
				case MESH_MSG_TYPE_REMOTE_SET_RE_REQ:	
										
					//set report
					operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_RESP;
					sprintf(operation.data,"%s",MESSAGE_DATA_CONFIG_RE_RESP_PASS);

					//do set re mode
					//status = check_and_set_re_mode();
					break;
				default:
					operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_RESP;
					sprintf(operation.data,"%s","Unknow command.");
					
			}
			
			sendto(client_sock,&operation,sizeof(operation),0,(struct sockaddr *)&server_addr,peerlen);
			printf("RE loop;send pass report  to CAP, operation.date = %s\n",operation.data);
			
	}
	
	close(client_sock);
	close(sockfd);
}

void * thread_re_run(void *param)
{
	char* mode;

	mode = get_mesh_mode();
	#if 0
	if(is_mesh_re_mode(mode))
	{
		sleep(15);
		execute_cmds("/etc/init.d/network restart");
	}
	else
	{
		pthread_mutex_lock(&simcom_mutex[PTHREAD_TYPE_RE]);
		pthread_cond_wait(&simcom_cond[PTHREAD_TYPE_RE], &simcom_mutex[PTHREAD_TYPE_RE]);
		pthread_mutex_unlock (&simcom_mutex[PTHREAD_TYPE_RE]);
	}
	#endif
	re_loop();

	pthread_exit(NULL);
}

