

/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement the main entry of  mesh configuration cmd as well as the 
framework of cmd config. These commands is for the use of wifison config.		

Author: Mr.Tsao Bo

Version: 0.3

Date: 2020-10-29

*****************************************************************************/

#include"mesh_config.h"

/*To be more explicit, manage all the cmds config in this array.*/
const struct mesh_cmd_struct mesh_cmd_config[MESH_CMD_MAX] = {
		{"capmode",	"capmode  		Set mode CAP\n",	handle_command_set_cap_mode},
		{"remode",	"remode			Set mode RE\n",	handle_command_set_re_mode},
		{"getmode",	"getmode			Get mode(CAP/RE/Nomal)\n",	handle_command_get_mode},
		{"showlink",	"showlink			Show link status\n",	handle_command_show_link_status},
		{"scan",		"scan			Scan wireless\n",	handle_command_scan_wireless},
		{"setssid",	"setssid			Set SSID\n",	handle_command_set_SSID},
		{"setpwd",	"setpwd			Set Password\n",		handle_command_set_password},
		{"rcre",		"rcre			Remote config RE. e.g.  rcre [ip adress] [ssid] [bssid]\n",		handle_command_remote_config_re},
		//Add new cmd here.
};


static inline int do_nothing (int argc, char *argv[])
{
    printf("options:%s is invalid.\n", argv[1]);
    /*Do nothing*/
    return 0;
}


static int usg(char **argv)
{
	printf("Usage: %s [options]\n", argv[0]);
	printf("Version:0.3\n");
	for (int i = 0; i < MESH_CMD_MAX; i++){
		printf(mesh_cmd_config[i].cmd_description);
	}
	
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

	if (status != 0){
       	return status;
	}

	return 0;
	
}
