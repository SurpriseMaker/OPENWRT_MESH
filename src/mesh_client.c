/*****************************************************************************

Description: This file implements the meshconfig client for remote connection.

Author: Mr.Tsao Bo

Version: 0.3

Date: 2020-11-25

*****************************************************************************/
#include"mesh_config.h"
#include"mesh_server.h"


int connect_to_remote_and_handle_resp( char* ip_address)
{
	int sockfd,recv_len;
	const int MAX_CONNECT_ATTEMPT_TIMES = 100;
	int current_attempt_times = 0;

	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	socklen_t peerlen = sizeof(server_addr);
	struct timeval tv_out;
	struct_mesh_msg operation={0};

	if((sockfd = socket(PF_INET,SOCK_STREAM,0))==-1)
	{
		perror("fail to create socket");
		return -1;
	}

	printf("Socket is created successfully.\n");
	
	bzero(&client_addr,sizeof(client_addr)); 
    	client_addr.sin_family = AF_INET;    
    	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    	client_addr.sin_port = htons(0); 

	if( bind(sockfd,(struct sockaddr*)&client_addr,sizeof(client_addr)))
    	{
        	printf("Bind Port Failed!\n"); 
		close(sockfd);
        	return -2;
    	}

	printf("Bind successfully.\n");
	
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(MESH_PROT);
	server_addr.sin_addr.s_addr=inet_addr(ip_address);

	while(1){
		if(connect(sockfd,(struct sockaddr*)&server_addr, peerlen) < 0)
    		{
    			current_attempt_times++;
			if(current_attempt_times > MAX_CONNECT_ATTEMPT_TIMES){
        			printf("Can Not Connect To Remote Device!,target ip =%s\n",ip_address);
				close(sockfd);
        			return -3;
			}
			sleep(2);
    		}else{
    			printf("Total connect time: %d s\n", current_attempt_times * 2);
    			break;
    		}
	}

	printf("Calibrating Void lenses.\n");
	
	operation.msg_type = MESH_MSG_TYPE_REMOTE_SET_RE_REQ;
	memcpy(operation.data,MESSAGE_DATA_CONFIG_RE_REQ,sizeof(MESSAGE_DATA_CONFIG_RE_REQ));

	if(send(sockfd,&operation,sizeof(operation),0)<0){
		printf("send failed !Annihilation commencing.\n");
		close(sockfd);
        	return -4;
	}

	printf("Synchronizing.\n");
		
	tv_out.tv_sec = 2;
	tv_out.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));

	memset(&operation,0,sizeof(operation));

	while( recv_len = recv(sockfd,&operation,sizeof(operation),0) > 0)
    	{
       	dbg_time("Recv: len = %d,data = %s\n",recv_len,operation.data);
		printf("Recv: len = %d,data = %s\n",recv_len,operation.data);

		if(operation.msg_type  == MESH_MSG_TYPE_REMOTE_SET_RE_RESP){
			if(strcmp(operation.data,MESSAGE_DATA_CONFIG_RE_RESP_PASS) == 0){
				printf("Recv: Config RE sucessfully.\n");
				printf("Assimilation successful.\n");
				break;
			}
		}

        	memset(&operation,0,sizeof(operation));  
    	}
		

	close(sockfd);
	return recv_len;
}

int remote_config_re(char* ip_address, char* bssid,char* ssid, char* password)
{
	int result;
	
	config_as_repeater_and_restart(ip_address,bssid,ssid,password);

	result = connect_to_remote_and_handle_resp(ip_address);

	config_restore_from_repeater();
	
	printf("result = %d\n",result);
	
	if(result > 0){
		printf("Successfully command remote device. 0(n_n)0\n");
	}else{
		printf("Failed to command remote device. :-(\n");
	}
	return 0;
}