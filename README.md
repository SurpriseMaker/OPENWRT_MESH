# OPENWRT_MESH
本方案是基于OPENWRT的WIFISON自组网控制指令。
支持设置UE角色（CAP/RE/Normal）、查看连接状态、搜索新设备、设置UE SSID和密码、
通过TCP连接远程控制设备组网、查看拓扑信息等。

## 指令一览：
### 指令名称：
meshconfig

### 指令选项:

#### mode.cap.auto		
设置device作为WIFISON的CAP角色,并自行组网

#### mode.cap.wps
设置device作为WIFISON的CAP角色，需按WPS键组网

#### mode.re.auto			
设置device作为WIFISON的RE角色，并自行组网

#### mode.re.wps
设置device作为WIFISON的RE角色，需按WPS键组网

#### mode.re.remote
远程设置RE，从主路由（CAP）通过无线连接将新设备设置为RE

举例：
mode.re.remote <backhaul id> <bssid> <ssid> [password]

远程设置RE指令封装了如下过程：
1. CAP作为STA连接到远端设备
2. CAP与远端设备建立TCP连接
3. CAP发送控制命令
4. 远端设备接收并执行命令后返回
5. CAP接收返回信息，将自己回退到CAP模式

#### mode.normal
回复到normal状态

#### mode.normal.remote
远程移除RE，从主路由（CAP）通过无线连接将已连接设备设置为normal，等效为移除已连接设备。

#### mode.get
获取device当前角色(CAP/RE/Nomal)

#### showlink		
显示ath当前连接状态

#### scan			
搜索并发现新设备

#### set.ssid			
设置本机SSID

#### set.pwd			
设置本机密码

#### topo.get
查询拓扑信息


### 用法：
系统调用，可ssh连接路由器后输入meshconfig，也可以在应用中调用。


### 服务名称
meshserver

### 用途：
作为远程控制的服务端，用于接收和执行远程控制指令。

### 用法：
开机自启动


## 文件目录
meshconfig/

|——files

      |——mesh.init  server初始化    
      |——topology.sh  拓扑信息查询脚本
      
|——src
      |——error_handle.c   异常处理
      |——error_handle.h
      |——logger.c   日志记录
      |——logger.h   
      |——Makefile
      |——mesh_client.c    远程通信的C端，发送控制指令，接收反馈
      |——mesh_config.h
      |——mesh_config_cap.c      负责CAP端设置
      |——mesh_config_details.h
      |——mesh_config_handle_command.c   负责指令解析
      |——mesh_config_main.c   主函数，负责指令识别与分发
      |——mesh_config_re.c    负责RE端设置
      |——mesh_config_repeater.c    repeater设置
      |——mesh_scan.c    负责搜索设备
      |——mesh_server.c  远程通信的S端，开辟单独线程
      |——mesh_server.h
      |——mesh_utils.c   通用API
      |——mesh_utils.h
      |——server_thread.c    S端子线程，接收并执行指令，返回结果
|——makefile

## 设计概述：
### 框架
![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/wifison%E8%BD%AF%E4%BB%B6%E6%9E%B6%E6%9E%84.PNG)

#### 清晰的分层结构
指令识别分发；
指令解析；
执行；
搜索；
C/S交互...
每一层实现单一职责，低耦合。

### 极简代码
7行代码实现主函数（对比OPENWRT原生，例如wlanconfig主函数100+行，同样的效果）

主函数：

int main(int argc, char *argv[])
{

	const char *cmd;
	int status = 0;
	int (*handle_command) (int argc, char *argv[]);

	if(argc < 2){
		return usg(argv);
	}

	cmd = argv[1];	
	handle_command = get_handle_command_function(cmd);	

	status = handle_command(argc, argv);

        return status;	
}

### 结构化组织
将指令名、指令说明、实现接口组织到一起，便于修改和查询。

/*To be more explicit, manage all the meshconfig commands in this array.*/
const mesh_cmd_struct mesh_cmd_config[MESH_CMD_MAX] = {
		{"mode.cap.auto",	"Config as CAP. e.g. mode.cap.auto [backhaul SSID]",	handle_command_set_mode_cap_auto},
		{"mode.cap.wps",	"Config as CAP and need press WPS button to connect.",		handle_command_set_cap_wps_mode},
		{"mode.re.auto",	"Config as RE. e.g. mode.re.auto [backhaul SSID]",	handle_command_set_mode_re_auto},
		{"mode.re.wps",		"Config as RE and need press WPS button to connect.",	handle_command_set_re_wps_mode},
		{"mode.re.remote",		"Remote config a device to be RE. e.g.  mode.re.remote  <backhaul SSID><bssid><ssid>[password]",		handle_command_remote_config_re},
		{"mode.normal",		"Restore to normal mode.", 		handle_command_restore_to_normal_mode},
		{"mode.normal.remote",	"Set remote device to normal mode. e.g. mode.normal.remote <ip address>",		handle_command_set_remote_device_to_normal_mode},
		{"mode.get",	"Get mode(CAP/RE/Nomal)",	handle_command_get_mode},
		{"showlink",	"Show link status",	handle_command_show_link_status},
		{"scan",		"Scan wireless",	handle_command_scan_wireless},
		{"set.ssid",	"Set SSID, e.g. set.ssid <SSID>",	handle_command_set_SSID},
		{"set.password",	"Set Password, e.g. set.password <password>",		handle_command_set_password},			
		{"topo.get",	"Get topology",	handle_command_get_topology},
		
		
		//Add new cmd above.
};

### 使用管道+shell实现telnet自动登录执行拓扑查询
ip="127.0.0.1"
port=7777

inputfile=in 
input1="td s2"
input2="exit"

rm -fr $inputfile    
mknod $inputfile p  

exec 8<>$inputfile 

telnet $ip $port <&8 &
sleep 1; echo $input1 >> $inputfile   
echo $input2 >> $inputfile 


更新日期：2021-1-8
作者：GentlemanTsao

# [WIFISON]路由器自组网控制指令开发使用手册

## 1.什么是WIFISON

WIFISON全称Wi-Fi self-organizing network。

高通Wi-Fi自组织网络（Wi-Fi-SON）使配置和维护Wi-Fi网络更简便。它允许无障碍安装Wi-Fi路由或电视/流媒体。它根据应用程序、频带和上下文动态选择最佳链接，并检测和纠正性能问题。Wi-Fi son为简化家庭网络而生，它包括：

自行配置

自行管理

自行恢复

自我防卫


## 2.WIFISON的优势：
![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/advantage.jpg)

## 3.WIFISON组网示意图

星型组网
![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/star1.jpg)

![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/star2.jpg)
