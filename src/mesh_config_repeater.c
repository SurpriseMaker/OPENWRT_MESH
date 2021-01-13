


/*****************************************************************************


Description: This file implement the details configuration of repeater	.

Author: Mr.Tsao Bo

Date: 2021-1-4

*****************************************************************************/

#include "mesh_utils.h"
#include "mesh_config_details.h"

int config_as_repeater_and_restart(char* ip_address,char* bssid,char* ssid,char* password)
{
	execute_cmds_2("uci set network.lan.ipaddr=", TEMP_IP_ADDRESS);
	execute_cmds_2("uci set network.lan.gateway=", ip_address);	
	execute_cmds("uci set network.lan.stp=1");
	execute_cmds("uci del network.wwan");
	execute_cmds("uci add network interface");
	execute_cmds("uci rename network.@interface[-1]=wwan");
	execute_cmds("uci set network.wwan.proto=dhcp");
	execute_cmds("uci commit network");


	execute_cmds("uci del wireless.link");
	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=link");
	execute_cmds("uci set wireless.link.device=wifi0");
	execute_cmds("uci set wireless.link.network=lan");
	execute_cmds("uci set wireless.link.mode=sta");
	execute_cmds("uci set wireless.link.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.link.wds=1");	
	execute_cmds_2("uci set wireless.link.bssid=", bssid);
	execute_cmds_2("uci set wireless.link.ssid=", ssid);
	
	if(NULL == password){
		execute_cmds("uci set wireless.link.encryption=none");
	}else{
		execute_cmds("uci set wireless.link.encryption=psk-mixed+tkip+aes");
		execute_cmds_2("uci set wireless.link.key=", password);
	}
	execute_cmds("uci commit wireless");

	execute_cmds_2("uci add_list dhcp.@dnsmasq[0].server=", ip_address);	
	execute_cmds("uci set dhcp.lan.ignore=1");
	execute_cmds("uci set dhcp.lan.ra_management=1");
	execute_cmds("uci commit dhcp");


	execute_cmds("uci set firewall.@zone[0].network=\"lan wwan\"");
	execute_cmds("uci commit firewall");

	dbg_time("Connecting...\n");
	execute_cmds("/etc/init.d/network restart");
	
	return 0;
}

int config_restore_from_repeater(char* ip_address)
{
	execute_cmds_2("uci set network.lan.ipaddr=", ip_address);
	execute_cmds("uci del network.lan.gateway");
	execute_cmds("uci del network.lan.stp");
	execute_cmds("uci del network.wwan");
	execute_cmds("uci commit network");

	execute_cmds("uci del wireless.link");
	execute_cmds("uci commit wireless");

	execute_cmds_2("uci del_list dhcp.@dnsmasq[0].server=", ip_address);
	execute_cmds("uci del dhcp.lan.ignore");
	execute_cmds("uci del dhcp.lan.ra_management");
	execute_cmds("uci commit dhcp");

	execute_cmds("uci set firewall.@zone[0].network=lan");
	execute_cmds("uci commit firewall");

	execute_cmds("/etc/init.d/network restart");
	return 0;
}

