#ifndef __COMMON_FUNCS_H_
#define __COMMON_FUNCS_H_

void print_station_info (struct station_info *info)
{
    os_printf("print_station_info\n\n");
    
    if (info != NULL) os_printf("bssid:" MACSTR", ip:" IPSTR "\n",
                                MAC2STR(info->bssid), IP2STR(&info->ip));
    else os_printf("Station_info is NULL\n\n");
}

#endif
