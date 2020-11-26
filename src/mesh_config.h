#ifndef SIMCOM_MESH_H
#define SIMCOM_MESH_H


#include "mesh_utils.h"

#define REMOTE_IP_ADDRESS	 "192.168.10.1"

#define MESH_CONFIG_VERSION "Version 0.5"

enum MESH_CMD{
	MESH_CMD_SET_CAP_MODE,
	MESH_CMD_SET_RE_MODE,
	MESH_CMD_GET_MODE,
	MESH_CMD_SHOW_LINK_STATUS,
	MESH_CMD_SCAN,
	MESH_CMD_SET_SSID,
	MESH_CMD_SET_PASSWORD,
	MESH_CMD_REMOTE_CONFIG_RE,
	
	//ADD new cmd above
	MESH_CMD_MAX	
};

struct mesh_cmd_struct{
	char cmd_name[20];
	char cmd_description[200];
	int (*handle_command) (int argc, char *argv[]);
};

typedef int (*handle_func_ptr)(int argc, char *argv[]);


int handle_command_set_cap_mode (int argc, char *argv[]);

int handle_command_set_re_mode (int argc, char *argv[]);

int handle_command_get_mode(int argc, char *argv[]);

int handle_command_show_link_status(int argc, char *argv[]);

int handle_command_scan_wireless(int argc, char *argv[]);

int handle_command_set_SSID(int argc, char *argv[]);

int handle_command_set_password(int argc, char *argv[]);
int handle_command_remote_config_re(int argc, char *argv[]);

int connect_to_remote_and_handle_resp( char* ip_address);
int remote_config_re(char* ip_address, char* bssid,char* ssid, char* password);
int check_and_set_cap_mode();
int check_and_set_re_mode();
void restore_from_cap_mode();
int config_as_repeater_and_restart(char* ip_address,char* bssid,char* ssid,char* password);
int config_restore_from_repeater();

char* mesh_scan(const char *cmd);
int simcom_mesh_list(const char *cmd);
#endif
