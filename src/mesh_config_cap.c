


/*****************************************************************************


Description: This file implement the details configuration of CAP	.

Author: Mr.Tsao Bo

Date: 2020-10-29

*****************************************************************************/

#include "mesh_utils.h"
#include "mesh_config_details.h"


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
	sleep(1);
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
	sleep(1);
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

int check_and_set_mode_cap_auto(char* backhaul_ssid)
{
	char* mode;
	int status = 0;

	mode = get_mesh_mode();

	if(is_mesh_re_mode(mode)){
		dbg_time("Can't set cap mode because current mode is re.");
		dbg_time("Now restore the re settings first.");
		restore_from_re_mode();
	}
	mesh_cap_delete_old_settings();
	config_as_cap_mode_and_restart(backhaul_ssid);
	
	return status;
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


bool is_mesh_cap_mode(char* mode)
{
	bool ret = false;
	
	if (! strncmp(mode,"ap",2)){
		ret =  true;
	} 

	return ret;
}