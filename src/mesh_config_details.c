


/*****************************************************************************

Copyright: 2011-2020, SIMCOM. Co., Ltd.

Description: This file implement the details configuration of wifison config	.

Author: Mr.Tsao Bo

Version: initial

Date: 2020-10-29

*****************************************************************************/

#include "mesh_utils.h"



void config_as_cap_mode(char* backhaul_ssid)
{
	execute_cmds("uci set wireless.wifi0.repacd_auto_create_vaps=0");
	execute_cmds("uci set wireless.wifi1.repacd_auto_create_vaps=0");


	execute_cmds("uci set wireless.@wifi-iface[0].wps_pbc='1'");
	execute_cmds("uci set wireless.@wifi-iface[1].wps_pbc='1'");

	execute_cmds("uci add wireless wifi-iface");
	execute_cmds("uci rename wireless.@wifi-iface[-1]=back1");
	execute_cmds("uci set wireless.back1.network=lan");
	execute_cmds("uci set wireless.back1.mode=ap");
	execute_cmds("uci set wireless.back1.wds=1");
	execute_cmds_2("uci set wireless.back1.ssid=",backhaul_ssid);
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
	execute_cmds_2("uci set wireless.back2.ssid=",backhaul_ssid);
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
}

void config_as_cap_mode_and_restart(char* backhaul_ssid){
	config_as_cap_mode(backhaul_ssid);
	
	execute_cmds("/etc/init.d/network restart");

	execute_cmds("/etc/init.d/repacd start");

}

void restore_from_cap_mode()
{
	execute_cmds("uci set wireless.@wifi-iface[0].wps_pbc=0");
	execute_cmds("uci set wireless.@wifi-iface[1].wps_pbc=0");
	execute_cmds("uci delete wireless.back1");
	execute_cmds("uci delete wireless.back2");
	execute_cmds("uci commit wireless");

	execute_cmds("uci set lbd.@config[0].Enable=0");
	execute_cmds("uci commit lbd");
	
	execute_cmds("uci set repacd.repacd.Enable=0");
	execute_cmds("uci set repacd.repacd.GatewayConnectedMode=AP");
	execute_cmds("uci set repacd.repacd.ConfigREMode=auto");
	execute_cmds("uci set repacd.repacd.DefaultREMode=qwrap");
	execute_cmds("uci commit repacd");

	execute_cmds("uci set hyd.@config[0].Enable=0");
	execute_cmds("uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=60");
	execute_cmds("uci commit hyd");

	execute_cmds("uci set wsplcd.config.HyFiSecurity=0");
	execute_cmds("uci commit wsplcd");

}

void restore_from_cap_mode_and_restart(){
	restore_from_cap_mode();

	execute_cmds("/etc/init.d/network restart");

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

void config_as_re_mode(char* backhaul_ssid)
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
	execute_cmds_2("uci set wireless.back1.ssid=", backhaul_ssid);
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
	execute_cmds_2("uci set wireless.back2.ssid=", backhaul_ssid);
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
	execute_cmds_2("uci set wireless.back3.ssid=", backhaul_ssid);
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
	execute_cmds_2("uci set wireless.back4.ssid=", backhaul_ssid);
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
	execute_cmds("uci set wsplcd.config.ConfigSta=0");
	execute_cmds("uci commit wsplcd");

}

void config_as_re_mode_and_restart(char* backhaul_ssid)
{
	config_as_re_mode(backhaul_ssid);

	execute_cmds("/etc/init.d/network restart");	
	
	execute_cmds("/etc/init.d/repacd start");
}

void restore_from_re_mode(){
	execute_cmds("uci set wireless.@wifi-iface[0].wps_pbc=0");
	execute_cmds("uci set wireless.@wifi-iface[1].wps_pbc=0");

	execute_cmds("uci delete wireless.back1");
	execute_cmds("uci delete wireless.back2");
	execute_cmds("uci delete wireless.back3");
	execute_cmds("uci delete wireless.back4");

	execute_cmds("uci del_list wireless.wifi0.ul_hyst=10");
	execute_cmds("uci commit wireless");

	execute_cmds("uci add network interface");
	execute_cmds("uci rename network.@interface[-1]=wan");
	execute_cmds("uci set network.wan.ifname=eth3");
	execute_cmds("uci set network.wan.proto=dhcp");

	execute_cmds("uci add network interface");
	execute_cmds("uci rename network.@interface[-1]=wan6");
	execute_cmds("uci set network.wan.ifname=eth3");
	execute_cmds("uci set network.wan.proto=dhcpv6");

	execute_cmds("uci set network.lan.proto=static");
	execute_cmds("uci set network.lan.ifname=\'eth1 eth2\'");
	execute_cmds("uci commit network");

	execute_cmds("uci del_list dhcp.lan.ignore=1");
	execute_cmds("uci commit dhcp");

	execute_cmds("uci set lbd.@config[0].Enable=0");
	execute_cmds("uci commit lbd");

	execute_cmds("uci set repacd.repacd.Enable=0");
	execute_cmds("uci set repacd.repacd.ConfigREMode=auto");
	execute_cmds("uci set repacd.repacd.DefaultREMode=qwrap");
       execute_cmds("uci set repacd.WiFiLink.ManageVAPInd=0");
	execute_cmds("uci set repacd.WiFiLink.DaisyChain=1");
	execute_cmds("uci commit repacd");

	execute_cmds("uci set hyd.@config[0].Enable=0");
	execute_cmds("uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1");
	execute_cmds("uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=60");
	execute_cmds("uci commit hyd");

	execute_cmds("uci set wsplcd.config.HyFiSecurity=0");
	execute_cmds("uci del_list wsplcd.config.ConfigSta=0");
	execute_cmds("uci commit wsplcd");
}

void restore_from_re_mode_and_restart(){
	restore_from_re_mode();

	execute_cmds("/etc/init.d/network restart");	
	
	execute_cmds("/etc/init.d/repacd start");
}

void mesh_re_delete_old_settings()
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

int check_and_set_cap_mode(char* backhaul_ssid)
{
	char* mode;
	int status = 0;

	mode = get_mesh_mode();

	if(is_mesh_re_mode(mode)){
		//TODO: We got to implement restore later.
		printf("Can't set cap mode because current mode is re.");
		printf("Now restore the re settings first.");
		restore_from_re_mode();
	}
	mesh_cap_delete_old_settings();
	config_as_cap_mode_and_restart(backhaul_ssid);
	
	return status;
}

int check_and_set_re_mode(char* backhaul_ssid)
{
	char* mode;
	int status = 0;

	mode = get_mesh_mode();

	if(is_mesh_cap_mode(mode)){
		printf("Can't set re mode because current mode is cap.");
		printf("Now restore the cap settings first.");
		restore_from_cap_mode();
	}
	mesh_re_delete_old_settings();
	config_as_re_mode_and_restart(backhaul_ssid);

	return status;
}

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

	printf("Connecting...\n");
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

int config_as_cap_wps_mode(void){
	execute_cmds("uci set wireless.qcawifi=qcawifi");
	execute_cmds("uci set wireless.qcawifi.wps_pbc_extender_enhance=1");
	execute_cmds("uci set wireless.qcawifi.wps_pbc_overwrite_ap_settings_all=1");
	execute_cmds("uci set wireless.qcawifi.wps_pbc_overwrite_sta_settings_all=1");
	execute_cmds("uci commit wireless");

	execute_cmds("uci set lbd.@config[0].Enable=1");
	execute_cmds("uci set lbd.StaDB.MarkAdvClientAsDualBand=1");
	execute_cmds("uci set lbd.SteerExec_Adv.StartInBTMActiveState=1");
	execute_cmds("uci set lbd.config.InactDetectionFromTx=1");
	execute_cmds("uci commit lbd");

	execute_cmds("uci set hyd.@config[0].Enable=1");
	execute_cmds("uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1");
	execute_cmds("uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15");
	execute_cmds("uci set hyd.IEEE1905Settings.AvoidDupRenew=1 ");
	execute_cmds("uci set hyd.IEEE1905Settings.AvoidDupTopologyNotification=1 ");
	execute_cmds("uci commit hyd");

	execute_cmds("uci set repacd.repacd.Enable=1");
	execute_cmds("uci set repacd.repacd.GatewayConnectedMode=CAP");
	execute_cmds("uci set repacd.repacd.ConfigREMode=son");
	execute_cmds("uci set repacd.repacd.DefaultREMode=son");
	execute_cmds("uci set repacd.WiFiLink.DaisyChain=1");
	execute_cmds("uci commit repacd");

	execute_cmds("uci set wsplcd.config.HyFiSecurity=1");
	execute_cmds("uci commit wsplcd");

	return 0;
}

int config_as_cap_wps_mode_and_restart(void){
	config_as_cap_wps_mode();
	execute_cmds("/etc/init.d/repacd start");
	return 0;
}

int config_as_re_wps_mode(void){
	execute_cmds("uci delete network.wan");
	execute_cmds("uci delete network.wan6");
	execute_cmds("uci set network.lan.proto=dhcp");
	execute_cmds("uci set network.lan.ifname='eth1 eth2 eth3'");
	execute_cmds("uci commit network");

	execute_cmds("uci set dhcp.lan.ignore=1");
	execute_cmds("uci commit dhcp");

	execute_cmds("uci set wireless.qcawifi=qcawifi");
	execute_cmds("uci set wireless.qcawifi.wps_pbc_extender_enhance=1");
	execute_cmds("uci set wireless.qcawifi.wps_pbc_overwrite_ap_settings_all=1");
	execute_cmds("uci set wireless.qcawifi.wps_pbc_overwrite_sta_settings_all=1");
	execute_cmds("uci set wireless.wifi0.ul_hyst=10");
	execute_cmds("uci commit wireless");

	execute_cmds("uci set lbd.@config[0].Enable=1");
	execute_cmds("uci set lbd.StaDB.MarkAdvClientAsDualBand=1");
	execute_cmds("uci set lbd.SteerExec_Adv.StartInBTMActiveState=1");
	execute_cmds("uci set lbd.config.InactDetectionFromTx=1");
	execute_cmds("uci commit lbd");


	execute_cmds("uci set repacd.repacd.DefaultREMode=son");
	execute_cmds("uci set repacd.WiFiLink.ManageVAPInd=1");
	execute_cmds("uci set repacd.repacd.Enable=1");
	execute_cmds("uci set repacd.repacd.ConfigREMode='son'");
	execute_cmds("uci set repacd.WiFiLink.DaisyChain=1");
	execute_cmds("uci set repacd.WiFiLink.5GBackhaulEvalTimeShort='180' ");
	execute_cmds("uci set repacd.WiFiLink.5GBackhaulEvalTimeLong='300' ");
	execute_cmds("uci set repacd.WiFiLink.2GBackhaulEvalTime='300'");
	execute_cmds("uci commit repacd");

	execute_cmds("uci set hyd.@config[0].Enable=1");
	execute_cmds("uci set hyd.config.Mode='HYCLIENT'");
	execute_cmds("uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1");
	execute_cmds("uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15");
	execute_cmds("uci set hyd.IEEE1905Settings.AvoidDupRenew=1 ");
	execute_cmds("uci set hyd.IEEE1905Settings.AvoidDupTopologyNotification=1 ");
	execute_cmds("uci commit hyd");

	execute_cmds("uci set wsplcd.config.HyFiSecurity=1");
	execute_cmds("uci commit wsplcd");

	return 0;
}

int config_as_re_wps_mode_and_restart(void){
	config_as_re_wps_mode();

	execute_cmds("/etc/init.d/network restart");
	execute_cmds("/etc/init.d/repacd start");

	return 0;
}