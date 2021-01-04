
CROSS-COMPILE:=arm-openwrt-linux-muslgnueabi-
CC := $(CROSS-COMPILE)gcc
#LD:=$(CROSS-COMPILE)ld

QL_MESH_OBJS:=mesh_config_main.o mesh_config_handle_command.o mesh_config_details.o mesh_client.o mesh_scan.o mesh_utils.o logger.o error_handle.o
QL_SVR_OBJS:=mesh_server.o logger.o mesh_utils.o server_thread.o mesh_config_details.o error_handle.o
TARGET:=meshserver meshconfig

release:$(TARGET)
	@echo $(TARGET) has been built!

meshserver:$(QL_SVR_OBJS)
	$(CC) -o $@ $^ -lpthread -ldl

meshconfig:$(QL_MESH_OBJS)
	$(CC) -o $@ $^ -lpthread -ldl

%.o:%.c
	$(CC) -Wall -c -o $@ $<

clean:
	rm -rf meshserver meshconfig *.o
