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
