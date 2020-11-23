


/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement the details configuration of wifison config	.

Author: Mr.Tsao Bo

Version: initial

Date: 2020-10-29

*****************************************************************************/

#include "mesh_utils.h"



static void mesh_config_as_cap()
{
	execute_cmds("uci set wireless.wifi0.repacd_auto_create_vaps=0");
	execute_cmds("uci set wireless.wifi1.repacd_auto_create_vaps=0");

	execute_cmds("uci set wireless.@wifi-iface[0].ssid=HISING_MESH");
	execute_cmds("uci set wireless.@wifi-iface[0].key=1234567890");
	execute_cmds("uci set wireless.@wifi-iface[0].encryption=psk2+ccmp");
	execute_cmds("uci set wireless.@wifi-iface[0].wps_pbc='1'");

	execute_cmds("uci set wireless.@wifi-iface[1].ssid=HISING_MESH");
	execute_cmds("uci set wireless.@wifi-iface[1].key=1234567890");
	execute_cmds("uci set wireless.@wifi-iface[1].encryption=psk2+ccmp");
	execute_cmds("uci set wireless.@wifi-iface[1].wps_pbc='1'");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back1");
	execute_cmds("uci set wireless.back1.network=lan");
	execute_cmds("uci set wireless.back1.mode=ap");
	execute_cmds("uci set wireless.back1.wds=1");
	execute_cmds("uci set wireless.back1.ssid=backhaul");
	execute_cmds("uci set wireless.back1.device=wifi0");
	execute_cmds("uci set wireless.back1.wsplcd_unmanaged=1");
	execute_cmds("uci set wireless.back1.repacd_security_unmanaged=1");
	execute_cmds("uci set wireless.back1.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.back1.key=1234567890");
	execute_cmds("uci set wireless.back1.wps_pbc='1'");
	execute_cmds("uci set wireless.back1.backhaul_ap=1");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back2");
	execute_cmds("uci set wireless.back2.network=lan");
	execute_cmds("uci set wireless.back2.mode=ap");
	execute_cmds("uci set wireless.back2.wds=1");
	execute_cmds("uci set wireless.back2.ssid=backhaul");
	execute_cmds("uci set wireless.back2.device=wifi1");
	execute_cmds("uci set wireless.back2.wsplcd_unmanaged=1");
	execute_cmds("uci set wireless.back2.repacd_security_unmanaged=1");
	execute_cmds("uci set wireless.back2.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.back2.key=1234567890");
	execute_cmds("uci set wireless.back2.wps_pbc='1'");
	execute_cmds("uci set wireless.back2.backhaul_ap=1");

	execute_cmds("uci commit wireless");

	execute_cmds("uci set lbd.@config[0].Enable=1");
	execute_cmds("uci commit lbd");

	execute_cmds("uci set repacd.repacd.Enable=1");
	execute_cmds("uci set repacd.repacd.GatewayConnectedMode=CAP");
	execute_cmds("uci set repacd.repacd.ConfigREMode=son");
	execute_cmds("uci set repacd.repacd.DefaultREMode=son");
	execute_cmds("uci set repacd.WiFiLink.DaisyChain=1");
	execute_cmds("uci commit repacd");

	execute_cmds("uci set hyd.@config[0].Enable=1");
	execute_cmds("uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1");
	execute_cmds("uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15");
	execute_cmds("uci commit hyd");

	execute_cmds("uci set wsplcd.config.HyFiSecurity=1");
	execute_cmds("uci commit wsplcd");

	execute_cmds("/etc/init.d/network restart");
	//TODO: sleep is not an appropriate way.
	execute_cmds("sleep 3");
	execute_cmds("/etc/init.d/repacd start");
}

static void mesh_cap_delete_old_settings()
{
	execute_cmds("uci delete wireless.wifi0.repacd_auto_create_vaps");
	execute_cmds("uci delete wireless.wifi1.repacd_auto_create_vaps");

	execute_cmds("uci delete wireless.@wifi-iface[0].wps_pbc");
	execute_cmds("uci delete wireless.@wifi-iface[1].wps_pbc");

	execute_cmds("uci delete wireless.back1");
	execute_cmds("uci delete wireless.back2");

	execute_cmds("uci commit wireless");
}

static void mesh_config_as_re()
{
	execute_cmds("uci set wireless.wifi0.repacd_auto_create_vaps=0");
	execute_cmds("uci set wireless.wifi1.repacd_auto_create_vaps=0");

	execute_cmds("uci set wireless.@wifi-iface[0].wps_pbc='1'");
	execute_cmds("uci set wireless.@wifi-iface[1].wps_pbc='1'");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back1");
	execute_cmds("uci set wireless.back1.network=lan");
	execute_cmds("uci set wireless.back1.mode=sta");
	execute_cmds("uci set wireless.back1.wds=1");
	execute_cmds("uci set wireless.back1.ssid=backhaul");
	execute_cmds("uci set wireless.back1.device=wifi0");
	execute_cmds("uci set wireless.back1.wsplcd_unmanaged=1");
	execute_cmds("uci set wireless.back1.repacd_security_unmanaged=1");
	execute_cmds("uci set wireless.back1.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.back1.key=1234567890");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back2");
	execute_cmds("uci set wireless.back2.network=lan");
	execute_cmds("uci set wireless.back2.mode=sta");
	execute_cmds("uci set wireless.back2.wds=1");
	execute_cmds("uci set wireless.back2.ssid=backhaul");
	execute_cmds("uci set wireless.back2.device=wifi1");
	execute_cmds("uci set wireless.back2.wsplcd_unmanaged=1");
	execute_cmds("uci set wireless.back2.repacd_security_unmanaged=1");
	execute_cmds("uci set wireless.back2.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.back2.key=1234567890");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back3");
	execute_cmds("uci set wireless.back3.network=lan");
	execute_cmds("uci set wireless.back3.mode=ap");
	execute_cmds("uci set wireless.back3.wds=1");
	execute_cmds("uci set wireless.back3.ssid=backhaul");
	execute_cmds("uci set wireless.back3.device=wifi0");
	execute_cmds("uci set wireless.back3.wsplcd_unmanaged=1");
	execute_cmds("uci set wireless.back3.repacd_security_unmanaged=1");
	execute_cmds("uci set wireless.back3.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.back3.key=1234567890");
	execute_cmds("uci set wireless.back3.backhaul_ap=1");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back4");
	execute_cmds("uci set wireless.back4.network=lan");
	execute_cmds("uci set wireless.back4.mode=ap");
	execute_cmds("uci set wireless.back4.wds=1");
	execute_cmds("uci set wireless.back4.ssid=backhaul");
	execute_cmds("uci set wireless.back4.device=wifi1");
	execute_cmds("uci set wireless.back4.wsplcd_unmanaged=1");
	execute_cmds("uci set wireless.back4.repacd_security_unmanaged=1");
	execute_cmds("uci set wireless.back4.encryption=psk2+ccmp");
	execute_cmds("uci set wireless.back4.key=1234567890");
	execute_cmds("uci set wireless.back4.backhaul_ap=1");

	execute_cmds("uci set wireless.wifi0.ul_hyst=10");
	execute_cmds("uci commit wireless");

	execute_cmds("uci delete network.wan");
	execute_cmds("uci delete network.wan6");
	execute_cmds("uci set network.lan.proto=dhcp");
	execute_cmds("uci set network.lan.ifname='eth1 eth2 eth3'");
	execute_cmds("uci commit network");

	execute_cmds("uci set dhcp.lan.ignore=1");
	execute_cmds("uci commit dhcp");

	execute_cmds("uci set lbd.@config[0].Enable=1");
	execute_cmds("uci commit lbd");

	execute_cmds("uci set repacd.repacd.Enable=1");
	execute_cmds("uci set repacd.repacd.ConfigREMode=son");
	execute_cmds("uci set repacd.repacd.DefaultREMode=son");
	execute_cmds("uci set repacd.WiFiLink.DaisyChain=1");
	execute_cmds("uci set repacd.WiFiLink.manageVAPInd=1");
	execute_cmds("uci commit repacd");

	execute_cmds("uci set hyd.@config[0].Enable=1");
	execute_cmds("uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1");
	execute_cmds("uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15");
	execute_cmds("uci commit hyd");

	execute_cmds("uci set wsplcd.config.HyFiSecurity=1");
	execute_cmds("uci commit wsplcd");

	execute_cmds("/etc/init.d/network restart");
	
	//TODO: sleep is not an appropriate way.
	sleep(15);
	execute_cmds("/etc/init.d/repacd start");
}

static void mesh_re_delete_old_settings()
{
	execute_cmds("uci delete wireless.wifi0.repacd_auto_create_vaps");
	execute_cmds("uci delete wireless.wifi1.repacd_auto_create_vaps");

	execute_cmds("uci delete wireless.@wifi-iface[0].wps_pbc");
	execute_cmds("uci delete wireless.@wifi-iface[1].wps_pbc");

	execute_cmds("uci delete wireless.back1");
	execute_cmds("uci delete wireless.back2");
	execute_cmds("uci delete wireless.back3");
	execute_cmds("uci delete wireless.back4");

	execute_cmds("uci commit wireless");
}

int check_and_set_cap_mode(){
	char* mode;
	int status = 0;

	mode = get_mesh_mode();

	if(is_mesh_re_mode(mode)){
		//TODO: We got to implement restore later.
		printf("Can't set cap mode because current mode is re.");
		printf("You should restore the re settings first.");
		status = -1;
	}else {
		mesh_cap_delete_old_settings();
		mesh_config_as_cap();
	}
	
	return status;
}

int check_and_set_re_mode(){
	char* mode;
	int status = 0;

	mode = get_mesh_mode();

	if(is_mesh_cap_mode(mode)){
		//TODO: We got to implement restore later.
		printf("Can't set re mode because current mode is cap.");
		printf("You should restore the cap settings first.");
		status = -1;
	}else {
		mesh_re_delete_old_settings();
		mesh_config_as_re();
	}
	return status;
}

int config_as_repeater(){
	execute_cmds("uci set network.lan.ipaddr=192.168.10.2");
	execute_cmds("uci set network.lan.gateway=192.168.10.1");
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
	execute_cmds("uci set wireless.link.encryption=none");
	execute_cmds("uci set wireless.link.ssid=ccc");
	execute_cmds("uci set wireless.link.wds=1");
	execute_cmds("uci set wireless.link.bssid=30:95:E3:3F:3B:5A");
	execute_cmds("uci commit wireless");

	execute_cmds("uci add_list dhcp.@dnsmasq[0].server=192.168.10.1");
	execute_cmds("uci set dhcp.lan.ignore=1");
	execute_cmds("uci set dhcp.lan.ra_management=1");
	execute_cmds("uci commit dhcp");

	execute_cmds("uci set firewall.@zone[0].network=\"lan wwan\"");
	execute_cmds("uci commit firewall");

	execute_cmds("/etc/init.d/network restart");
	
	return 0;
}

int config_restore_from_repeater(){
	execute_cmds("uci set network.lan.ipaddr=192.168.10.1");
	execute_cmds("uci del network.lan.gateway");
	execute_cmds("uci del network.lan.stp");
	execute_cmds("uci del network.wwan");
	execute_cmds("uci commit network");

	execute_cmds("uci del wireless.link");
	execute_cmds("uci commit wireless");

	execute_cmds("uci del_list dhcp.@dnsmasq[0].server=192.168.10.1");
	execute_cmds("uci del dhcp.lan.ignore");
	execute_cmds("uci del dhcp.lan.ra_management");
	execute_cmds("uci commit dhcp");

	execute_cmds("uci set firewall.@zone[0].network=lan");
	execute_cmds("uci commit firewall");

	execute_cmds("/etc/init.d/network restart");

	sleep(3000);
	return 0;
}
