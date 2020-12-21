#ifndef SIMCOM_MESH_H
#define SIMCOM_MESH_H


#include "mesh_utils.h"
#include "mesh_config_details.h"

enum MESH_CMD{
	MESH_CMD_SET_CAP_MODE,
	MESH_CMD_SET_RE_MODE,
	MESH_CMD_GET_MODE,
	MESH_CMD_SHOW_LINK_STATUS,
	MESH_CMD_SCAN,
	MESH_CMD_SET_SSID,
	MESH_CMD_SET_PASSWORD,
	MESH_CMD_REMOTE_CONFIG_RE,
	MESH_CMD_RESTORE_TO_NORMAL_MODE,
	MESH_CMD_GET_TOPOLOGY,
	
	//ADD new cmd above
	MESH_CMD_MAX	
};

typedef struct {
	char cmd_name[20];
	char cmd_description[200];
	int (*handle_command) (int argc, char *argv[]);
}mesh_cmd_struct;

typedef struct {
	char* ip_address;
	char* bssid;
	char* ssid;
	char* password;
}remote_device_info_struct;

typedef int (*handle_func_ptr)(int argc, char *argv[]);


int handle_command_set_cap_mode (int argc, char *argv[]);

int handle_command_set_re_mode (int argc, char *argv[]);

int handle_command_get_mode(int argc, char *argv[]);

int handle_command_show_link_status(int argc, char *argv[]);

int handle_command_scan_wireless(int argc, char *argv[]);

int handle_command_set_SSID(int argc, char *argv[]);

int handle_command_set_password(int argc, char *argv[]);
int handle_command_remote_config_re(int argc, char *argv[]);
int handle_command_restore_to_normal_mode(int argc, char *argv[]);
int handle_command_get_topology(int argc, char *argv[]);

int connect_to_remote_and_handle_resp(char* backhaul_ssid, char* ip_address);
int remote_config_re(char* backhaul_ssid,remote_device_info_struct* remote_info);
char* mesh_scan(const char *cmd);
int simcom_mesh_list(const char *cmd);
char* get_topology(void);
#endif
