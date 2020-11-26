

/*****************************************************************************

Description: This file implements the main entry of  mesh configuration commands as well as
the framework of meshconfig commands. These commands is for the use of wifison config.		

Author: Mr.Tsao Bo

Date: 2020-11-26

*****************************************************************************/

#include"mesh_config.h"

/*To be more explicit, manage all the meshconfig commands in this array.*/
const struct mesh_cmd_struct mesh_cmd_config[MESH_CMD_MAX] = {
		{"capmode",	"Config as CAP",	handle_command_set_cap_mode},
		{"remode",	"Config as RE",	handle_command_set_re_mode},
		{"getmode",	"Get mode(CAP/RE/Nomal)",	handle_command_get_mode},
		{"showlink",	"Show link status",	handle_command_show_link_status},
		{"scan",		"Scan wireless",	handle_command_scan_wireless},
		{"setssid",	"Set SSID",	handle_command_set_SSID},
		{"setpwd",	"Set Password",		handle_command_set_password},
		{"rcre",		"Remote config RE. e.g.  rcre  <bssid><ssid>[password]",		handle_command_remote_config_re},

		//Add new cmd above.
};


static inline int do_nothing (int argc, char *argv[])
{
    printf("options:%s is invalid.\n", argv[1]);
    /*Do nothing*/
    return 0;
}


static int usg(char **argv)
{
	printf("%s\n",MESH_CONFIG_VERSION);
	printf("-----------------------------------------------------------------\n");
	printf("Usage: %s [options]\n", argv[0]);
	
	for (int i = 0; i < MESH_CMD_MAX; i++){
		printf("%s  		%s\n",mesh_cmd_config[i].cmd_name ,mesh_cmd_config[i].cmd_description);
	}

	printf("-----------------------------------------------------------------\n");
	return 0;
}

handle_func_ptr get_handle_command_function(const char * cmd){
	handle_func_ptr handle_command;
	
	handle_command = do_nothing;
	for(int i = 0; i < MESH_CMD_MAX; i++){
		struct mesh_cmd_struct cmd_item = mesh_cmd_config[i];
		
		if(streq(cmd,cmd_item.cmd_name)){
			handle_command = cmd_item.handle_command;
			break;
		}
	}

	return handle_command;
}
	
int main(int argc, char *argv[])
{
	const char *cmd;
	int status = 0;
	int (*handle_command) (int argc, char *argv[]);

	if(argc < 2){
		return usg(argv);
	}

	cmd = argv[1];	
	handle_command = get_handle_command_function(cmd);	

	status = handle_command(argc, argv);

       return status;	
}
