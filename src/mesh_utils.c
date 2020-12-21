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


#include    <sys/wait.h>  
#include    <errno.h>  

static pid_t    *childpid = NULL;  
                        /* ptr to array allocated at run-time */  
static int      maxfd;  /* from our open_max(), {Prog openmax} */  

#define SHELL   "/bin/sh"  

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


FILE *  my_popen(const char *cmdstring, const char *type)  
{  
    int     i, pfd[2];  
    pid_t   pid;  
    FILE    *fp;  

            /* only allow "r" or "w" */  
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {  
        errno = EINVAL;     /* required by POSIX.2 */  
        return(NULL);  
    }  

    if (childpid == NULL) {     /* first time through */  
                /* allocate zeroed out array for child pids */  
        maxfd = 1024;  
        if ( (childpid = calloc(maxfd, sizeof(pid_t))) == NULL)  
            return(NULL);  
    }  

    if (pipe(pfd) < 0)  
        return(NULL);   /* errno set by pipe() */  

    if ( (pid = fork()) < 0)  
        return(NULL);   /* errno set by fork() */  
    else if (pid == 0) {                            /* child */  
        if (*type == 'r') {  
            close(pfd[0]);  
            if (pfd[1] != STDOUT_FILENO) {  
                dup2(pfd[1], STDOUT_FILENO);  
                close(pfd[1]);  
            }  
        } else {  
            close(pfd[1]);  
            if (pfd[0] != STDIN_FILENO) {  
                dup2(pfd[0], STDIN_FILENO);  
                close(pfd[0]);  
            }  
        }  
            /* close all descriptors in childpid[] */  
        for (i = 0; i < maxfd; i++)  
            if (childpid[ i ] > 0)  
                close(i);  

	
        execl(SHELL, "sh", "-c", cmdstring, (char *) 0);  

        _exit(127);  
    }  
                                /* parent */  
    if (*type == 'r') {  
        close(pfd[1]);  
        if ( (fp = fdopen(pfd[0], type)) == NULL)  
            return(NULL);  
    } else {  
        close(pfd[0]);  
        if ( (fp = fdopen(pfd[1], type)) == NULL)  
            return(NULL);  
    }  
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */  
    return(fp);  
}


int  my_pclose(FILE *fp)  
{  

    int     fd, stat;  
    pid_t   pid;  

    if (childpid == NULL)  
        return(-1);     /* popen() has never been called */  

    fd = fileno(fp);  
    if ( (pid = childpid[fd]) == 0)  
        return(-1);     /* fp wasn't opened by popen() */  

    childpid[fd] = 0;  
    if (fclose(fp) == EOF)  
        return(-1);  

    while (waitpid(pid, &stat, 0) < 0)  
        if (errno != EINTR)  
            return(-1); /* error other than EINTR from waitpid() */  

    return(stat);   /* return child's termination status */  
}  