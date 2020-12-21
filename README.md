# OPENWRT_MESH
实现了OPENWRT下WIFISON自组网，带一个server用于接收远程命令，将设备连接到现有WIFISON网络中。

指令：meshconfig

options:

capmode			--设置device作为WIFISON的CAP角色

remode			--设置device作为WIFISON的RE角色

getmode			--获取device当前角色(CAP/RE/Nomal)

showlink		-- 显示ath当前连接状态

scan			--搜索并发现新设备

setssid			--设置本机SSID

setpwd			--设置本机密码

rcre			--远程控制，通过TCP连接将新设备设置作为RE

e.g. rcre <bssid> <ssid> [password]

Remote config RE via TCP steps:（远程控制步骤）
1. CAP set as STA mode to connect remote device
2. CAP build TCP connection to remote device
3. CAP send control message to remote device
4. Remote device receive and execute control command,respond
5. CAP receives response message and reset itself as CAP mode.



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
