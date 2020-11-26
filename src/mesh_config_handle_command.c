
/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement the meshconfig command handler functions.

Author: Mr.Tsao Bo

Version: 0.2

Date: 2020-11-23

*****************************************************************************/
#include"mesh_config.h"
#include"mesh_server.h"


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
	
	scan_result = mesh_scan("iw dev ath0 scan");
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
	char* bssid;
	char* ssid;
	char* password;
	
	if(argc > 3){
		bssid = argv[2];
		ssid = argv[3];
		if(argc > 4){
			password = argv[4];
		}else{
			password = NULL;
		}
		
		remote_config_re(REMOTE_IP_ADDRESS,bssid,ssid,password);		
	}	
	return 0;
}

