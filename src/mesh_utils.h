#ifndef MESH_UTILS_H
#define MESH_UTILS_H

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

/* Version Rules:
 *  Format: Release.Major.Minor.Patch
 *  Major: When API compatible changes
 *  Minior: When more functions are added
 *  Patch:  When fix issues or  get improved.
 */
#define MESH_CONFIG_VERSION "Version 0.9.9.6"

#define DEFAULT_IP_ADDRESS	 "192.168.3.1"
#define TEMP_IP_ADDRESS "192.168.3.2"

#define streq(a,b) ((strlen(a) == strlen(b)) && (strncasecmp(a,b,strlen(b)) == 0))

char* my_system(const char *cmd);
void execute_cmds(char *arg);
void execute_cmds_2(char *arg1, char *arg2);


char* get_mesh_mode();

FILE *  my_popen(const char *cmdstring, const char *type)  ;
int  my_pclose(FILE *fp)  ;
#endif
