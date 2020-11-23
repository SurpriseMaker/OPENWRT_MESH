uci set wireless.wifi0.repacd_auto_create_vaps=0
uci set wireless.wifi1.repacd_auto_create_vaps=0

uci set wireless.@wifi-iface[0].wps_pbc='1'
uci set wireless.@wifi-iface[1].wps_pbc='1'

uci add wireless wifi-iface
uci rename wireless.@wifi-iface[-1]=back1
uci set wireless.back1.network=lan
uci set wireless.back1.mode=sta
uci set wireless.back1.wds=1
uci set wireless.back1.ssid=backhaultest
uci set wireless.back1.device=wifi0
uci set wireless.back1.wsplcd_unmanaged=1
uci set wireless.back1.repacd_security_unmanaged=1

uci add wireless wifi-iface
uci rename wireless.@wifi-iface[-1]=back2
uci set wireless.back2.network=lan
uci set wireless.back2.mode=sta
uci set wireless.back2.wds=1
uci set wireless.back2.ssid=backhaultest
uci set wireless.back2.device=wifi1
uci set wireless.back2.wsplcd_unmanaged=1
uci set wireless.back2.repacd_security_unmanaged=1

uci add wireless wifi-iface
uci rename wireless.@wifi-iface[-1]=back3
uci set wireless.back3.network=lan
uci set wireless.back3.mode=ap
uci set wireless.back3.wds=1
uci set wireless.back3.ssid=backhaultest
uci set wireless.back3.device=wifi0
uci set wireless.back3.wsplcd_unmanaged=1
uci set wireless.back3.repacd_security_unmanaged=1
uci set wireless.back3.backhaul_ap=1

uci add wireless wifi-iface
uci rename wireless.@wifi-iface[-1]=back4
uci set wireless.back4.network=lan
uci set wireless.back4.mode=ap
uci set wireless.back4.wds=1
uci set wireless.back4.ssid=backhaultest
uci set wireless.back4.device=wifi1
uci set wireless.back4.wsplcd_unmanaged=1
uci set wireless.back4.repacd_security_unmanaged=1
uci set wireless.back4.backhaul_ap=1

uci set wireless.wifi0.ul_hyst=10
uci commit wireless

uci delete network.wan
uci delete network.wan6
uci set network.lan.proto=dhcp
uci set network.lan.ifname='eth1 eth2 eth3'
uci set network.lan.gateway='192.168.2.1'
uci commit network

uci set dhcp.lan.ignore=1
uci commit dhcp

uci set lbd.@config[0].Enable=1
uci commit lbd

uci set repacd.repacd.Enable=1
uci set repacd.repacd.GatewayConnectedMode=CAP
uci set repacd.repacd.ConfigREMode=son
uci set repacd.repacd.DefaultREMode=son
uci set repacd.WiFiLink.DaisyChain=1
uci commit repacd

uci set hyd.@config[0].Enable=1
uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1
uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15
uci commit hyd

uci set wsplcd.config.HyFiSecurity=1
uci commit wsplcd

/etc/init.d/network restart
