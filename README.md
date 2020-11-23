# OPENWRT_MESH
实现了OPENWRT下WIFISON自组网，带一个server用于接收远程命令，将设备连接到现有WIFISON网络中。

指令：meshconfig
options:
capmode			Set mode CAP--设置device作为WIFISON的CAP角色
remode			Set mode RE --设置device作为WIFISON的RE角色
getmode			Get mode(Controller/Agent Nomal)--获取device当前角色
showlink		Show link status -- 显示ath当前连接状态
scan			Scan wireless --搜索并发现新设备
setssid			Set SSID --设置SSID
setpwd			Set Password --设置密码
rcre			远程控制，通过TCP连接将新设备设置作为RE
==============================================================
Remote config RE via TCP steps:
1. CAP set as STA mode to connect remote device
2. CAP build TCP connection to remote device
3. CAP send control message to remote device
4. Remote device receive and execute control command,respond
5. CAP receives response message and reset itself as CAP mode.
==============================================================


更新日期：2020-11-23
