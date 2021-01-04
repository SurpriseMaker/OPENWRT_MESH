/*****************************************************************************

Description: This file implements the meshconfig client for remote connection.

Author: Mr.Tsao Bo

Date: 2020-11-25

*****************************************************************************/
#include"mesh_config.h"
#include"mesh_server.h"
#include "error_handle.h"

int connect_to_remote_and_handle_resp(char* backhaul_ssid, char* ip_address)
{
	int sockfd,recv_len;
	int current_attempt_times = 0;

	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	socklen_t peerlen = sizeof(server_addr);
	struct timeval tv_out;
	struct_mesh_msg send_operation={0};
	struct_mesh_msg recv_operation={0};

	if((sockfd = socket(PF_INET,SOCK_STREAM,0))==-1){
		perror("fail to create socket");
		return ERROR_CREATE_SOCKET_FAILED;
	}

	printf("Socket is created successfully.\n");
	
	bzero(&client_addr,sizeof(client_addr)); 
    	client_addr.sin_family = AF_INET;    
    	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    	client_addr.sin_port = htons(0); 

	if( bind(sockfd,(struct sockaddr*)&client_addr,sizeof(client_addr))){
        	printf("Bind Port Failed!\n"); 
		close(sockfd);
        	return ERROR_BIND_FAILED;
    	}

	printf("Bind successfully.\n");
	
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(MESH_PROT);
	server_addr.sin_addr.s_addr=inet_addr(ip_address);

	while(1){
		if(connect(sockfd,(struct sockaddr*)&server_addr, peerlen) < 0){
    			current_attempt_times++;
			if(current_attempt_times > MAX_CONNECT_ATTEMPT_TIMES){
        			printf("Can Not Connect To Remote Device!,target ip =%s\n",ip_address);
				close(sockfd);
        			return ERROR_CONNECT_REMOTE_FAILED;
			}
			sleep(1);
    		}else{
    			printf("Total connect time: %d s\n", current_attempt_times );
    			break;
    		}
	}

	printf("Calibrating Void lenses.\n");

	memset(&send_operation,0,sizeof(send_operation));
	send_operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_REQ;
	strcat(send_operation.data,backhaul_ssid);

	printf("send: operation.msg_type=%d, operation.data=%s\n",send_operation.msg_type,send_operation.data);
	
	if(send(sockfd,&send_operation,sizeof(send_operation),0)<0){
		printf("send failed !Annihilation commencing.\n");
		close(sockfd);
        	return ERROR_SEND_FAILED;
	}

	printf("Synchronizing.\n");
		
	tv_out.tv_sec = 2;
	tv_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));

	memset(&recv_operation,0,sizeof(recv_operation));

	while( (recv_len = recv(sockfd,&recv_operation,sizeof(recv_operation),0)) > 0)
    	{
       	dbg_time("Recv: len = %d,data = %s\n",recv_len,recv_operation.data);
		printf("Recv: len = %d,data = %s\n",recv_len,recv_operation.data);

		if(recv_operation.msg_type  == MESH_MSG_TYPE_REMOTE_SET_RE_RESP){
			printf("Verify server received data =%s\n", recv_operation.verifydata);

			if(strcmp(recv_operation.verifydata,send_operation.data) != 0){
				printf("Warning!!!: Data Verified Failed.There are transmission error.\n");
			}
			
			if(strcmp(recv_operation.data,MESSAGE_DATA_CONFIG_RE_RESP_PASS) == 0){
				printf("Recv: Config RE sucessfully.\n");
				printf("Assimilation successful.\n");
				break;
			}
		}

        	memset(&recv_operation,0,sizeof(recv_operation));  
    	}
		

	close(sockfd);
	return recv_len;
}

int remote_config_re(char* backhaul_ssid,remote_device_info_struct* remote_info)
{
	int result =0;
	int recv_len = 0;
	char* error_string;
	
	config_as_repeater_and_restart(remote_info->ip_address,
									remote_info->bssid,
									remote_info->ssid,
									remote_info->password);

	recv_len = connect_to_remote_and_handle_resp(backhaul_ssid,remote_info->ip_address);

	config_restore_from_repeater(remote_info->ip_address);
	
	printf("recv_len = %d\n",recv_len);
	
	if(recv_len > 0){
		printf("Successfully command remote device. 0(n_n)0\n");
		result = 0;
	}else{
		printf("Failed to command remote device. :-(\n");
		
		result = recv_len;
		error_string = error_code_to_string(result);
		printf("ERROR:%s\n",error_string);
	}
	return result;
}