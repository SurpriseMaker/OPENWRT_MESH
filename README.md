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




更新日期：2020-11-26
作者：GentlemanTsao

[WIFISON]路由器自组网控制指令开发使用手册

1.什么是WIFISON

WIFISON全称Wi-Fi self-organizing network。

高通Wi-Fi自组织网络（Wi-Fi-SON）使配置和维护Wi-Fi网络更简便。它允许无障碍安装Wi-Fi路由或电视/流媒体。它根据应用程序、频带和上下文动态选择最佳链接，并检测和纠正性能问题。Wi-Fi son为简化家庭网络而生，它包括：

自行配置

自行管理

自行恢复

自我防卫


2.WIFISON的优势：
![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/advantage.jpg)

3.WIFISON组网示意图

星型组网
![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/star1.jpg)

![Image text](https://github.com/SurpriseMaker/OPENWRT_MESH/blob/main/pics/star2.jpg)
