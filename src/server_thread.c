
/*****************************************************************************

Description: This file implements a server thread to receive and proceed commands.

Author: Mr.Tsao Bo

Date: 2020-11-25

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
	struct_mesh_msg recv_operation={0};
	struct_mesh_msg send_operation={0};
	char* backhaul_ssid;

	dbg_time("server loop init.\n");
	printf("---------------------------------------------------------------------------\n");
	printf("Server loop init.\n");
	printf("The server listens commands from remote device and executes them.\n");
	printf("e.g. when receives MESH_MSG_TYPE_REMOTE_SET_RE_REQ message from remote,\n");
	printf("it will configure the device as a RE role.\n");	
	printf("---------------------------------------------------------------------------\n");

	if((sockfd = socket(PF_INET,SOCK_STREAM,0))==-1){
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
	
	if(listen(sockfd,10)==-1){
		perror("listen failed");
		printf("listen failed");
		close(sockfd);
		pthread_exit(NULL);
	}
	printf("Prismatic core online..\n");
	
	while(1){
		printf("Systems at full.\n");
		client_sock = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&clientaddr_size);
        	if (client_sock < 0){
            		perror("accept failed");
			printf("Accept failed,Andakur herak.\n");
           	 	pthread_exit(NULL);
        	}
		
		memset(&recv_operation,0,sizeof(recv_operation));
		recv_len = recvfrom(client_sock,&recv_operation,sizeof(recv_operation),0,(struct sockaddr*)&client_addr,&peerlen);
			
		printf("Rcv msg :recv_len=%d, msg_type=%d, data=%s\n",recv_len,recv_operation.msg_type,recv_operation.data);
		dbg_time("Rcv msg :recv_len=%d,msg_type=%d, data=%s\n",recv_len, recv_operation.msg_type, recv_operation.data);

		memset(&send_operation,0,sizeof(send_operation));
		switch(recv_operation.msg_type){
			case MESH_MSG_TYPE_REMOTE_SET_RE_REQ:	
				backhaul_ssid = recv_operation.data;
					
				send_operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_RESP;
				strcat(send_operation.data,MESSAGE_DATA_CONFIG_RE_RESP_PASS);
				strcat(send_operation.verifydata,recv_operation.data);
					
				sendto(client_sock,&send_operation,sizeof(send_operation),0,(struct sockaddr *)&server_addr,peerlen);
				printf("Processed.\n send report  to CAP, data = %s\n",send_operation.data);
			
				check_and_set_mode_re_auto(backhaul_ssid);
				break;
				
			case MESH_MSG_TYPE_REMOTE_RESTORE_RE_REQ:
				send_operation.msg_type = MESH_MSG_TYPE_REMOTE_RESTORE_RE_RESP;
				strcat(send_operation.data,MESSAGE_DATA_CONFIG_RE_RESP_PASS);
				strcat(send_operation.verifydata,recv_operation.data);

				sendto(client_sock,&send_operation,sizeof(send_operation),0,(struct sockaddr *)&server_addr,peerlen);
				printf("Processed.\n send report  to CAP, data = %s\n",send_operation.data);

				restore_from_re_mode_and_restart();
				break;
				
			default:

				send_operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_RESP;
				strcat(send_operation.data,MESSAGE_DATA_CONFIG_RE_RESP_UNKNOW);
				strcat(send_operation.verifydata,recv_operation.data);

				sendto(client_sock,&send_operation,sizeof(send_operation),0,(struct sockaddr *)&server_addr,peerlen);
				printf("Processed.\n send  report  to CAP, data = %s\n",send_operation.data);
					
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

