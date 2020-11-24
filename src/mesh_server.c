
/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement the mesh server to simulate auto mesh config.

Author: Mr.Tsao Bo

Version: initial

Date: 2020-10-29

*****************************************************************************/

#include "mesh_utils.h"
#include "mesh_server.h"



int main(int argc, const char **argv)
{
	int error;
	
	error =pthread_create(&mesh_tid, NULL, mesh_server_run, NULL);

	if(0 != error){
		printf("can't create thread  %s",strerror(error));
		exit(1);
	}

	pthread_join(mesh_tid, NULL);
		
	return 0;
}

