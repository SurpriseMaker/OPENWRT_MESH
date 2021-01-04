
#include "error_handle.h"

char*  error_code_to_string(mesh_connect_error_code error_code){

	switch(error_code){
		case ERROR_CREATE_SOCKET_FAILED:
			return STRING_CREATE_SOCKET_FAILED;
		case ERROR_BIND_FAILED:
			return STRING_BIND_FAILED;
		case ERROR_CONNECT_REMOTE_FAILED:
			return STRING_CONNECT_REMOTE_FAILED;
		case ERROR_SEND_FAILED:
			return STRING_SEND_FAILED;
		default:
			return STRING_UNKNOW_ERROR;
	}
}
