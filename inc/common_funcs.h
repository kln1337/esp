#ifndef __COMMON_FUNCS_H_
#define __COMMON_FUNCS_H_

#include <stdio.h>
#include "ets_sys.h"
#include "user_interface.h"
#include "gpio.h"
#include "osapi.h"
#include "driver/uart.h"
#include "user_config.h"
#include "mem.h"
#include "ip_addr.h"
#include "espconn.h"

void print_station_info (struct station_info *info);

void uint32_to_array_uint8 (uint32 ptr, uint8 array[4]);

void print_ip(uint8 ip[4]);

void print_tcp_ip(struct espconn* econn);

void print_data(char *pdata, unsigned short len);
#endif
