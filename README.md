# OPENWRT_MESH
本方案是基于OPENWRT的WIFISON自组网控制指令。
支持设置UE角色（CAP/RE/Normal）、查看连接状态、搜索新设备、设置UE SSID和密码、
通过TCP连接远程控制设备组网、查看拓扑信息等。

## 指令一览：
### 指令名称：
meshconfig

### 指令选项:

#### capmode		
设置device作为WIFISON的CAP角色

#### remode			
设置device作为WIFISON的RE角色

#### rn
回复到normal状态

#### getmode			
获取device当前角色(CAP/RE/Nomal)

#### showlink		
显示ath当前连接状态

#### scan			
搜索并发现新设备

#### setssid			
设置本机SSID

#### setpwd			
设置本机密码

#### rcre			
远程控制，通过TCP连接将新设备设置作为RE

举例：
rcre <bssid> <ssid> [password]

Remote config RE via TCP steps:（远程控制步骤）
1. CAP作为STA连接到远端设备
2. CAP与远端设备建立TCP连接
3. CAP发送控制命令
4. 远端设备接收并执行命令后返回
5. CAP接收返回信息，将自己回退到CAP模式

#### gettopo
获取拓扑信息

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

      |——logger.c   日志记录
      
      |——logger.h   
      
      |——Makefile
      
      |——mesh_client.c    远程通信的C端，发送控制指令，接收反馈
      
      |——mesh_config.h
      
      |——mesh_config_details.c      负责指令具体执行
      
      |——mesh_config_details.h
      
      |——mesh_config_handle_command.c   负责指令解析
      
      |——mesh_config_main.c   主函数，负责指令识别与分发
      
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
const mesh_cmd_struct mesh_cmd_config[MESH_CMD_MAX] = {
		{"capmode",	"Config as CAP. e.g. capmode <backhaul SSID>",	handle_command_set_cap_mode},
		{"remode",	"Config as RE. e.g. remode <backhaul SSID>",	handle_command_set_re_mode},
		{"getmode",	"Get mode(CAP/RE/Nomal)",	handle_command_get_mode},
		{"showlink",	"Show link status",	handle_command_show_link_status},
		{"scan",		"Scan wireless",	handle_command_scan_wireless},
		{"setssid",	"Set SSID, e.g. setssid <SSID>",	handle_command_set_SSID},
		{"setpwd",	"Set Password, e.g. setpwd <password>",		handle_command_set_password},
		{"rcre",		"Remote config RE. e.g.  rcre  <backhaul SSID><bssid><ssid>[password]",		handle_command_remote_config_re},
		{"normalmode",		"Restore to normal mode.", 		handle_command_restore_to_normal_mode},
		{"gettopo",	"Get topology",	handle_command_get_topology},
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


更新日期：2020-12-24
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
