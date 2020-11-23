
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
	int error_cap, error_re;
	
	for(int i = 0;i < PTHREAD_TYPE_NUM ;i++)
	{
		pthread_mutex_init (&simcom_mutex[i], NULL);
		pthread_cond_init (&simcom_cond[i], NULL);
	}

	
	error_cap = pthread_create(&simcom_tid[PTHREAD_TYPE_CAP], NULL, thread_cap_run, NULL);
	if(0 != error_cap){
		printf("can't create thread %d : %s",PTHREAD_TYPE_CAP,strerror(error_cap));
		exit(1);
	}
	
	error_re =pthread_create(&simcom_tid[PTHREAD_TYPE_RE], NULL, thread_re_run, NULL);
	if(0 != error_re){
		printf("can't create thread %d : %s",PTHREAD_TYPE_RE,strerror(error_re));
		exit(1);
	}


	pthread_join(simcom_tid[PTHREAD_TYPE_CAP], NULL);
		
	return 0;
}

