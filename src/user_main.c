#include <stdio.h>
#include "ets_sys.h"
#include "user_interface.h"
#include "gpio.h"
#include "osapi.h"
#include "/home/kln/prog/esp/ESP8266_NONOS_SDK-2.2.1/driver_lib/include/driver/uart.h"
#define LED 2

uint32 priv_param_start_sec;

void delay(uint32_t count)
{
    while(--count);
}

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            priv_param_start_sec = 0x3C;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            priv_param_start_sec = 0x7C;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
            rf_cal_sec = 512 - 5;
            priv_param_start_sec = 0x7C;
            break;
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            priv_param_start_sec = 0xFC;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
            rf_cal_sec = 1024 - 5;
            priv_param_start_sec = 0x7C;
            break;
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            priv_param_start_sec = 0xFC;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            priv_param_start_sec = 0xFC;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            priv_param_start_sec = 0xFC;
            break;
        default:
            rf_cal_sec = 0;
            priv_param_start_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}
/* void ICACHE_FLASH_ATTR user_pre_init(void) */
/* { */
/*   bool rc = false; */
/*   static const partition_item_t part_table[] =  */
/*   { */
/*     {SYSTEM_PARTITION_RF_CAL, */
/*      0x3fb000, */
/*      0x1000}, */
/*     {SYSTEM_PARTITION_PHY_DATA, */
/*      0x3fc000, */
/*      0x1000}, */
/*     {SYSTEM_PARTITION_SYSTEM_PARAMETER, */
/*      0x3fd000, */
/*      0x3000}, */
/*   }; */

/*   // This isn't an ideal approach but there's not much point moving on unless */
/*   // or until this has succeeded cos otherwise the SDK will just barf and  */
/*   // refuse to call user_init() */
/*   while (!rc) */
/*   { */
/*     rc = system_partition_table_regist(part_table, */
/* 				       sizeof(part_table)/sizeof(part_table[0]), */
/*                                        4); */
/*   } */

/*   return; */
/* } */

/* void user_spi_flash_dio_to_qio_pre_init(void) */
/* { */
/*     // EMPTY */
/* } */

void ICACHE_FLASH_ATTR user_init(void)
{
    gpio_init();
    UartBautRate uart0_br = BIT_RATE_115200;
    UartBautRate uart1_br = BIT_RATE_115200;
    uart_init(uart0_br, uart1_br);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    gpio_output_set(0, 0, (1 << LED),0);
    while(1) {
        // gpio_output_set(0, (1 << LED), 0, 0);
        os_delay_us(1000000);
        //system_soft_wdt_stop();
        //delay(1000000000);
        gpio_output_set((1 << LED), 0, 0, 0);
        os_delay_us(1000000);
        os_printf("HELLO WORLD!!!!!!!!!!!!");
        //os_printf("SDK version:%s\n", system_get_sdk_version());
    }
    
    //enum flash_size_map size_map = system_get_flash_size_map();
}
