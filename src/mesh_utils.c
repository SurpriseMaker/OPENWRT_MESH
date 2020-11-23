#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <net/if.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/inotify.h>
#include <signal.h>
#include "logger.h"


char* my_system(const char *cmd)
{
	static char cmd_result[25600] = {0};
	char buf[32] = {0};
	FILE *fp = NULL;

	memset(cmd_result,0,sizeof(cmd_result));
	if( (fp = popen(cmd, "r")) == NULL )
	{
		printf("popen error!\n");
	}else{
		while (fgets(buf, sizeof(buf), fp))
		{
			printf(buf);
			strcat(cmd_result, buf);
		}

		pclose(fp);
	}
	
	return cmd_result;
}

#if 0
void mysystem(const char *cmd)
{
	system(cmd);
}
#endif

void execute_cmds(char *cmds)
{
	char shell_cmd[256] = {'\0'};
	snprintf(shell_cmd, sizeof(shell_cmd), "%s", cmds);
	my_system(shell_cmd);
}

bool is_mesh_cap_mode(char* mode){
	bool ret = false;
	
	if (! strncmp(mode,"ap",2)){
		ret =  true;
	} 

	return ret;
}

bool is_mesh_re_mode(char* mode){
	bool ret = false;
	
	if (! strncmp(mode,"sta",3)){
		ret =  true;
	} 

	return ret;
}

char* get_mesh_mode(){
	return my_system("uci get wireless.back1.mode 2> /dev/null");
}

