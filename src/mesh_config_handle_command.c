
/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement the meshconfig command handler functions.

Author: Mr.Tsao Bo

Version: 0.2

Date: 2020-11-23

*****************************************************************************/
#include"mesh_config.h"
#include"mesh_server.h"


int mesh_send_message(mesh_msg_type_enum msg_type, char* ip_address, int mesh_port)
{
	int sockfd,recv_len;

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

	bzero(&client_addr,sizeof(client_addr)); 
    	client_addr.sin_family = AF_INET;    
    	client_addr.sin_addr.s_addr = htons(INADDR_ANY);//get local IP address.
    	client_addr.sin_port = htons(0); 

	if( bind(sockfd,(struct sockaddr*)&client_addr,sizeof(client_addr)))
    	{
        	printf("Client Bind Port Failed!\n"); 
        	exit(1);
    	}
	
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(mesh_port);
	server_addr.sin_addr.s_addr=inet_addr(ip_address);

	if(connect(sockfd,(struct sockaddr*)&server_addr, peerlen) < 0)
    	{
        	printf("Can Not Connect To Remote Device!\n");
        	exit(1);
    	}
	
	operation.msg_type = msg_type;

	switch(operation.msg_type)
	{
		case MESH_MSG_TYPE_REMOTE_SET_RE_REQ:
		{
			memcpy(operation.data,MESSAGE_DATA_CONFIG_RE_REQ,sizeof(MESSAGE_DATA_CONFIG_RE_REQ));
			break;
		}

		default:
			break;
	}

	if(send(sockfd,&operation,sizeof(operation),0)<0){
		printf("send failed !\n");
        	exit(1);
	}

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
				break;
			}
		}

        	memset(&operation,0,sizeof(operation));  
    	}
		

	close(sockfd);
	return recv_len;
}

int remote_config_re(char* ip_address)
{

	config_as_repeater();
	sleep(20);
	
	mesh_send_message(MESH_MSG_TYPE_REMOTE_SET_RE_REQ,ip_address,MESH_PROT);

	return 0;
}


int handle_command_set_cap_mode (int argc, char *argv[]){
	return check_and_set_cap_mode();
}

int handle_command_set_re_mode (int argc, char *argv[]){
	return check_and_set_re_mode();
	
}

int handle_command_get_mode(int argc, char *argv[]){
	char* mode;

	mode = get_mesh_mode();

	dbg_time("handle_command_get_mode: mode = %d",mode);
	if(is_mesh_cap_mode(mode))
	{
		printf("CAP\n");
	}
	else if(is_mesh_re_mode(mode))
	{
		printf("RE\n");
	}
	else
	{
		printf("Normal\n");
	}

	return 0;
}

int handle_command_show_link_status(int argc, char *argv[]){
	int status;
	printf("5G_MESH:\n");
	simcom_mesh_list("wlanconfig ath01 list");
	
	printf("2.4G_MESH:\n");
	status = simcom_mesh_list("wlanconfig ath11 list");
	//TODO:manage status meaning later.
	return status;
}

int handle_command_scan_wireless(int argc, char *argv[]){
	char* scan_result;
	
	scan_result = mesh_scan("iw dev ath1 scan");
	printf(scan_result);
	return 0;
}

int handle_command_set_SSID(int argc, char *argv[]){
	char cmd[64] = {0};
	const char *ssid;
	int result;

	if(argc > 2){
		ssid = argv[2];
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"uci set wireless.@wifi-iface[0].ssid=%s",ssid);
		execute_cmds(cmd);

		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"uci set wireless.@wifi-iface[1].ssid=%s",ssid);
		execute_cmds(cmd);
		
		execute_cmds("uci commit wireless");
		execute_cmds("/etc/init.d/network restart");
		result = 0;	
	}else{
		printf("aguement not set !\n");
		result = -1;
	}
	
	return result;
}

int handle_command_set_password(int argc, char *argv[]){
	char cmd[64] = {0};
	const char *password;
	int result;

	if(argc > 2){
		password = argv[2];
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"uci set wireless.@wifi-iface[0].key=%s",password);
		execute_cmds(cmd);

		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"uci set wireless.@wifi-iface[1].key=%s",password);
		execute_cmds(cmd);

		execute_cmds("uci commit wireless");
		execute_cmds("/etc/init.d/network restart");
	}else{
		printf("aguement not set !\n");
		result = -1;
	}
	
	return result;
}


int handle_command_remote_config_re(int argc, char *argv[]){
	char* ip_address;
	
	if(argc > 2){
		ip_address = argv[2];
		remote_config_re(ip_address);
	}	
	return 0;
}

