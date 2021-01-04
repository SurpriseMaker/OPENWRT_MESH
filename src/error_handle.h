#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H

#define STRING_CREATE_SOCKET_FAILED "Failed to create socket!"
#define STRING_BIND_FAILED "Bind failed!"
#define STRING_CONNECT_REMOTE_FAILED "Failed to connect to remote device!"
#define STRING_SEND_FAILED "Failed to send message to remote device !"
#define STRING_UNKNOW_ERROR "Unknow error."

typedef enum{
	ERROR_CREATE_SOCKET_FAILED = -1,
	ERROR_BIND_FAILED = -2,
	ERROR_CONNECT_REMOTE_FAILED = -3,
	ERROR_SEND_FAILED = -4,
}mesh_connect_error_code;

char*  error_code_to_string(mesh_connect_error_code error_code);

#endif	