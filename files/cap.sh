uci set wireless.wifi0.repacd_auto_create_vaps=0
uci set wireless.wifi1.repacd_auto_create_vaps=0


uci set wireless.@wifi-iface[0].ssid=njw_0
uci set wireless.@wifi-iface[0].key=1234567890
uci set wireless.@wifi-iface[0].encryption=psk2+ccmp
uci set wireless.@wifi-iface[0].wps_pbc='1'


uci set wireless.@wifi-iface[1].ssid=njw_0
uci set wireless.@wifi-iface[1].key=1234567890
uci set wireless.@wifi-iface[1].encryption=psk2+ccmp
uci set wireless.@wifi-iface[1].wps_pbc='1'

uci add wireless wifi-iface
uci rename wireless.@wifi-iface[-1]=back1
uci set wireless.back1.network=lan
uci set wireless.back1.mode=ap
uci set wireless.back1.wds=1
uci set wireless.back1.ssid=backhaultest
uci set wireless.back1.device=wifi0
uci set wireless.back1.wsplcd_unmanaged=1
uci set wireless.back1.repacd_security_unmanaged=1
uci set wireless.back1.encryption=psk2+ccmp
uci set wireless.back1.key=1234567890
uci set wireless.back1.wps_pbc='1'
uci set wireless.back1.backhaul_ap=1

uci add wireless wifi-iface
uci rename wireless.@wifi-iface[-1]=back2
uci set wireless.back2.network=lan
uci set wireless.back2.mode=ap
uci set wireless.back2.wds=1
uci set wireless.back2.ssid=backhaultest
uci set wireless.back2.device=wifi1
uci set wireless.back2.wsplcd_unmanaged=1
uci set wireless.back2.repacd_security_unmanaged=1
uci set wireless.back2.encryption=psk2+ccmp
uci set wireless.back2.key=1234567890
uci set wireless.back2.wps_pbc='1'
uci set wireless.back2.backhaul_ap=1

uci commit wireless

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
