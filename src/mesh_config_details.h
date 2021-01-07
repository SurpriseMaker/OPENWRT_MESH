#ifndef MESH_CONFIG_DETAILS_H
#define MESH_CONFIG_DETAILS_H

#define DEFAULT_BACKHAUL_SSID "backmesh"

int check_and_set_mode_re_auto(char* backhaul_ssid);
int check_and_set_mode_cap_auto(char* backhaul_ssid);

void restore_from_cap_mode();
void restore_from_cap_mode_and_restart();
void restore_from_re_mode();
void restore_from_re_mode_and_restart();
int config_as_repeater_and_restart(char* ip_address,char* bssid,char* ssid,char* password);
int config_restore_from_repeater(char* ip_address);
int config_as_cap_wps_mode_and_restart(void);
int config_as_re_wps_mode_and_restart(void);

bool is_mesh_cap_mode(char* mode);
bool is_mesh_re_mode(char* mode);
#endif