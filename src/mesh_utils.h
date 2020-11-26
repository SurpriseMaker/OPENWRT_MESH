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

#define MESH_CONFIG_VERSION "Version 0.5"

#define streq(a,b) ((strlen(a) == strlen(b)) && (strncasecmp(a,b,strlen(b)) == 0))

char* my_system(const char *cmd);
void execute_cmds(char *cmds);

bool is_mesh_cap_mode(char* mode);
bool is_mesh_re_mode(char* mode);
char* get_mesh_mode();
#endif
