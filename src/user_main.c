#include <stdio.h>
#include <common_funcs.h>
#include "ets_sys.h"
#include "user_interface.h"
#include "gpio.h"
#include "osapi.h"
#include "driver/uart.h"
#include "user_config.h"
#include "mem.h"
#include "ip_addr.h"
#include "espconn.h"

#define LED 2
#define SSID "esp8266"
#define PSWD "123456789"

uint8 led_status = 0;
os_timer_t os_timer; // watch struct ETSTimer (ets_sys.h)
struct softap_config wifi_config;
struct espconn srv_conf;

//my_client client  = {.number = 0, .ip = 0};

uint8 start_page[] = "<!DOCTYPE html>\n<html>\n<body>\n<h1>My First Heading</h1>\n<p>My first paragraph.</p>\n\n</body>\n</html>";

void espconn_connect_cb(void* arg)
{
    os_printf("Client ");
    print_ip(srv_conf.proto.tcp->remote_ip);
    os_printf("Port: %d\n\n", srv_conf.proto.tcp->remote_port);
    espconn_send(&srv_conf, start_page, sizeof(start_page) / sizeof(uint8));
}

void tcp_recv_cb(void *arg, char *pdata, unsigned short len)
{
    struct espconn *client = (struct espconn*)arg;
    os_printf("Recv_callback\n");
    os_printf("Data lenght: %d\n\n", len);
    print_tcp_ip(client);
    print_data(pdata, len);

    // espconn_send(&srv_conf, pdata, len);
}

void tcp_sent_cb(void *arg)
{
    
}

void tcp_reconnect_cb(void *arg, sint8 err)
{
    os_printf("reconnect: error code> %d\n", err);
}

static void ICACHE_FLASH_ATTR tcp_server_start(void)
{
    // prepare server ip address
    struct ip_info ip_addr_srv;
    wifi_get_ip_info(STATION_IF, &ip_addr_srv);
    
    // initializating main stuct for server
    srv_conf.type = ESPCONN_TCP;
    srv_conf.state = ESPCONN_NONE;
    srv_conf.proto.tcp = (esp_tcp *) os_zalloc(sizeof(esp_tcp));
    srv_conf.proto.tcp->local_port = 80;
    uint32_to_array_uint8(ip_addr_srv.ip.addr, srv_conf.proto.tcp->local_ip);
    // set callback func for connect
    srv_conf.proto.tcp->connect_callback = espconn_connect_cb;
    // set callback func for recive data
    espconn_regist_recvcb(&srv_conf, tcp_recv_cb);
    // set callback func for sent data
    espconn_regist_sentcb(&srv_conf, tcp_sent_cb);
    // set callback func for reconnect
    espconn_regist_reconcb(&srv_conf, tcp_reconnect_cb);
    espconn_accept(&srv_conf);
    os_printf("Server");
    print_ip(srv_conf.proto.tcp->local_ip);
}

void timer_handler(void)
{
    struct ip_info ip_addr; 
   
    // if we have client flashing led
    if (wifi_softap_get_station_num()) {
        led_status = !led_status;
        GPIO_OUTPUT_SET(LED, led_status);
        
        struct station_info *now_station_info = wifi_softap_get_station_info();

        /* os_printf("Client info\n"); */
        /* print_station_info(now_station_info); */
    }   
}

uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    uint32 priv_param_start_sec;

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

void ICACHE_FLASH_ATTR user_rf_pre_init(void)
{
}

void ICACHE_FLASH_ATTR user_init(void)
{
    int i = 0;
    gpio_init();
    
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
       
    system_soft_wdt_stop(); // stop watch dog
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    
    // work with timer
    os_timer_disarm(&os_timer); // reset struct os_timer_t
    os_timer_setfn(&os_timer, (os_timer_func_t*)timer_handler, NULL); // set handler func
    os_timer_arm(&os_timer, 5000, 1); // set time for timer 

    // Wifi setup
    wifi_set_opmode_current(SOFTAP_MODE); // set wifi like slave
    os_memset(&wifi_config, 0, sizeof(struct softap_config)); // reset wifi_config
    wifi_config.authmode = AUTH_WPA2_PSK; // set auth
    wifi_config.max_connection = 5;
    os_sprintf(wifi_config.ssid, "%s", SSID); // set ssid
    os_sprintf(wifi_config.password, "%s", PSWD); // set password

    
    // Appying settings 
    if (wifi_softap_set_config_current(&wifi_config)){
        // TCP server
        tcp_server_start();
    } else
        os_printf("SOFTAP_MODE didn't set");
}


