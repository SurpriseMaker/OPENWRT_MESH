
/*****************************************************************************

Description: This file implement a thread acting as RE to simulate remote config.

Author: Mr.Tsao Bo

Version: 0.3

Date: 2020-11-24

*****************************************************************************/

#include "mesh_utils.h"
#include "mesh_config_details.h"
#include "mesh_server.h"


void server_loop()
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

	dbg_time("server loop init.\n");
	printf("---------------------------------------------------------------------------\n");
	printf("Server loop init.\n");
	printf("The server listens commands from remote device and executes them.\n");
	printf("e.g. when receives MESH_MSG_TYPE_REMOTE_SET_RE_REQ message from remote,\n");
	printf("it will configure the device as a RE role.\n");
	printf("version:0.3\n");
	printf("---------------------------------------------------------------------------\n");

	if((sockfd = socket(PF_INET,SOCK_STREAM,0))==-1)
	{
		perror("fail to create socket");
		pthread_exit(NULL);
	}

	printf("Socket is created successfully.\n");
	
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(MESH_PROT);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        if ((result = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
		printf("[%s] bind : %d\n", __FUNCTION__, result);
            close(sockfd);
            pthread_exit(NULL);
        }

	printf("Bind successfully.\n");
	
	if(listen(sockfd,10)==-1)
	{
		perror("listen failed");
		printf("listen failed");
		pthread_exit(NULL);
	}
	printf("Prismatic core online..\n");
	
	while(1)
	{
		printf("Systems at full.\n");
		client_sock = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&clientaddr_size);
        	if (client_sock < 0)
        	{
            		perror("accept failed");
			printf("Accept failed,Andakur herak.\n");
           	 	pthread_exit(NULL);
        	}
		
			memset(&operation,0,sizeof(operation));
			recv_len = recvfrom(client_sock,&operation,sizeof(operation),0,(struct sockaddr*)&client_addr,&peerlen);
			
			printf("Rcv msg :recv_len=%d, operation.data=%s\n",recv_len,operation.data);
			dbg_time("Rcv msg :recv_len=%d, operation.data=%s\n",recv_len,operation.data);

			memset(&operation,0,sizeof(operation));
			switch(operation.msg_type)
			{
				case MESH_MSG_TYPE_REMOTE_SET_RE_REQ:	
										
					//set report
					operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_RESP;
					sprintf(operation.data,"%s",MESSAGE_DATA_CONFIG_RE_RESP_PASS);
					
					sendto(client_sock,&operation,sizeof(operation),0,(struct sockaddr *)&server_addr,peerlen);
					printf("Processed.\n send report  to CAP, operation.data = %s\n",operation.data);
			
					//do set re mode
					status = check_and_set_re_mode();
					break;
				default:
					operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_RESP;
					sprintf(operation.data,"%s","Unknow command.");

					sendto(client_sock,&operation,sizeof(operation),0,(struct sockaddr *)&server_addr,peerlen);
					printf("Processed.\n send  report  to CAP, operation.data = %s\n",operation.data);
					
			}
			
			
			
	}
	
	close(client_sock);
	close(sockfd);
}

void * mesh_server_run(void *param)
{
	server_loop();

	pthread_exit(NULL);
}

