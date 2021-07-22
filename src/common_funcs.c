#include <common_funcs.h>

void print_station_info(struct station_info *info)
{
    if (info != NULL) os_printf("bssid:" MACSTR", ip:" IPSTR "\n",
                                MAC2STR(info->bssid), IP2STR(&info->ip));
    else os_printf("Station_info is NULL\n\n");
}

void uint32_to_array_uint8(uint32 ptr, uint8 array[4])
{
    int i;
    for (i = 0; i < 4; i++)
        array[i] = (uint8)(ptr >> 8);
}

void print_ip(uint8 ip[4])
{
    if (ip != NULL)
        os_printf("IP:%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
}

void print_tcp_ip(struct espconn* econn)
{
    os_printf("Local ");
    print_ip(econn->proto.tcp->local_ip);
    os_printf("Remote ");
    print_ip(econn->proto.tcp->remote_ip);
}

void print_data(char *pdata, unsigned short len)
{
    for (uint16 i = 0; i < len; i++) {
        os_printf("%c", pdata++);
    }
    os_printf("\n\n");
}
