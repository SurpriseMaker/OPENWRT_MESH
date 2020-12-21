
/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implements the mesh server to simulate auto mesh config.

Author: Mr.Tsao Bo

Date: 2020-10-29

*****************************************************************************/

#include "mesh_utils.h"
#include "mesh_server.h"


static pthread_t mesh_tid = 0;

int main(int argc, const char **argv)
{
	int error;

	printf("%s\n",MESH_CONFIG_VERSION);
	
	error =pthread_create(&mesh_tid, NULL, mesh_server_run, NULL);

	if(0 != error){
		printf("can't create thread  %s",strerror(error));
		exit(1);
	}

	pthread_join(mesh_tid, NULL);
		
	return 0;
}

