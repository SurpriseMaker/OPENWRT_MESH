#include"mesh_config.h"


static char* filter_buffer_oneline(char *buffer)
{
	const char *filter_pattern[] = {
		"BSS ",
		"signal:",
		"SSID:",
	};

	static char filter_result[256] = {0};
	memset(filter_result,0,sizeof(filter_result));

	for (int i = 0; i < sizeof(filter_pattern) / sizeof(filter_pattern[0]) ; i++)
	{
		if (strstr(buffer, filter_pattern[i]))
		{
			strcat(filter_result,buffer);
			break;
		}
	}
	return filter_result;
}

char* mesh_scan(const char *cmd)
{
	char buf[0xff] = {0};
	FILE *fp = NULL;
	static char scan_result[16384] = {0};
	char* filter_oneline_result;
	
	memset(scan_result,0,sizeof(scan_result));

	if( (fp = popen(cmd, "r")) == NULL ) {
		perror("popen error!\n");
	}else {
		while (fgets(buf, sizeof(buf), fp)) {
			filter_oneline_result = filter_buffer_oneline(buf);
			strcat(scan_result,filter_oneline_result);	
		}
		pclose(fp);
	}
	
	return scan_result;
}

static void parse_list_buffer(int *index,char *buffer)
{
	int i;

	const char *filter_list_string[] = {
		"Max STA phymode"
	};

	for (i = 0; i < sizeof(filter_list_string) / sizeof(filter_list_string[0]); i++)
	{
		if (strstr(buffer, filter_list_string[i]))
		{
			*index = 0;
		}
	}

	if(*index == 1)
	{
		printf("%s",buffer);
	}

	(*index)++;
}

int simcom_mesh_list(const char *cmd)
{
	char buf[0xff] = {0};
	int index = 0;
	FILE *fp = NULL;

	if( (fp = popen(cmd, "r")) == NULL ) {
		perror("popen error!\n");
		return -1;
	}

	while (fgets(buf, sizeof(buf), fp)) {
		parse_list_buffer(&index,buf);
	}

	pclose(fp);
	
	return 0;
}
