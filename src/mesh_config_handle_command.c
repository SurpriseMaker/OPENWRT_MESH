
/*****************************************************************************

Description: This file implements the meshconfig command handler functions.

Author: Mr.Tsao Bo

Date: 2020-11-23

*****************************************************************************/
#include"mesh_config.h"
#include"mesh_server.h"


int handle_command_set_mode_cap_auto (int argc, char *argv[]){
	char* backhaul_ssid;

	if(argc > 2){
		backhaul_ssid = argv[2];
	}else{
		backhaul_ssid = DEFAULT_BACKHAUL_SSID;
	}

	printf("para: backhaul_ssid =%s \n",backhaul_ssid);
	return check_and_set_mode_cap_auto(backhaul_ssid);
}

int handle_command_set_mode_re_auto (int argc, char *argv[]){
	char* backhaul_ssid;

	if(argc > 2){
		backhaul_ssid = argv[2];
	}else{
		backhaul_ssid = DEFAULT_BACKHAUL_SSID;
	}

	printf("para: backhaul_ssid =%s \n",backhaul_ssid);
	return check_and_set_mode_re_auto(backhaul_ssid);
	
}

int handle_command_get_mode(int argc, char *argv[]){
	char* mode;

	mode = get_mesh_mode();

	dbg_time("handle_command_get_mode: mode = %d",mode);
	if(is_mesh_cap_mode(mode)){
		printf("CAP\n");
	}
	else if(is_mesh_re_mode(mode)){
		printf("RE\n");
	}
	else{
		printf("Normal\n");
	}

	return 0;
}

int handle_command_show_link_status(int argc, char *argv[]){
	int status;
	printf("5G_MESH:\n");
	status = simcom_mesh_list("wlanconfig ath01 list");
	
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
	char *ssid;
	int result = 0;

	if(argc > 2){
		ssid = argv[2];
		execute_cmds_2("uci set wireless.@wifi-iface[0].ssid=", ssid);
		execute_cmds_2("uci set wireless.@wifi-iface[1].ssid=", ssid);
		
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
	char *password;
	int result = 0;

	if(argc > 2){
		password = argv[2];
		execute_cmds_2("uci set wireless.@wifi-iface[0].key=", password);
		execute_cmds_2("uci set wireless.@wifi-iface[1].key=", password);

		execute_cmds("uci commit wireless");
		execute_cmds("/etc/init.d/network restart");
	}else{
		printf("aguement not set !\n");
		result = -1;
	}
	
	return result;
}


int handle_command_remote_config_re(int argc, char *argv[]){
	remote_device_info_struct remote_info={0};
	int result = 0;
	
	if(argc > 4){
		remote_info.backhaul_ssid=argv[2];
		remote_info.bssid = argv[3];
		remote_info.ssid = argv[4];
		if(argc > 5){
			remote_info.password = argv[5];
		}else{
			remote_info.password = NULL;
		}

		remote_info.ip_address = DEFAULT_IP_ADDRESS;

		result = remote_config_re(&remote_info);		
	} else {
		printf("Paremeters too few.\n");
		result = -1;
	}
	return result;
}

int handle_command_restore_to_normal_mode(int argc, char *argv[]){
	char* mode;

	mode = get_mesh_mode();

	dbg_time("handle_command_get_mode: mode = %d",mode);

	if(is_mesh_cap_mode(mode)){
		printf("Current mode is CAP mode,now restoring to normal...\n");
		restore_from_cap_mode_and_restart();
	} else if(is_mesh_re_mode(mode)){
		printf("Current mode is RE mode,now restoring to normal...\n");
		restore_from_re_mode_and_restart();
	}else{
		printf("Current mode is normal mode, it's unnecessary to restore.\n");
	}

	return 0;
}

int handle_command_set_remote_device_to_normal_mode(int argc, char *argv[]){
	remote_device_info_struct remote_info={0};
	int result = 0;

	if(argc > 2){
		remote_info.ip_address = argv[2];
	}else{
		printf("Paremeters too few.\n");
		result = -1;
	}

	remote_restore_re(&remote_info);

	return result;
}

int handle_command_get_topology(int argc, char *argv[]){
	char* scan_result;
	
	scan_result = get_topology();

	printf("%s\n",scan_result);
	return 0;
}

int handle_command_set_cap_wps_mode(int argc, char *argv[]){
	return config_as_cap_wps_mode_and_restart();
}

int handle_command_set_re_wps_mode(int argc, char *argv[]){
	return config_as_re_wps_mode_and_restart();
}