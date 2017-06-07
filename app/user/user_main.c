#include "esp_common.h"
#include "user_config.h"
#include "pwm.h"

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_GPIO2_U
#define PWM_0_OUT_IO_NUM 2
#define PWM_0_OUT_IO_FUNC FUNC_GPIO2

#define PWM_NUM_CHANNEL_NUM 1

uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void servo_set(uint8_t pwm_channel, uint16_t degrees) {
    pwm_set_duty(51 + (degrees/7), pwm_channel);
    pwm_start();
}

void ICACHE_FLASH_ATTR user_init(void)
{
    uart_div_modify(0, UART_CLK_FREQ / 115200);

    os_printf("SDK version:%s\n", system_get_sdk_version());

    wifi_set_opmode(0);

    uint32 io_info[][3] = {
        {PWM_0_OUT_IO_MUX,PWM_0_OUT_IO_FUNC,PWM_0_OUT_IO_NUM},
    };

    u32 duty[5] = {77};

    pwm_init(20000, duty ,PWM_NUM_CHANNEL_NUM,io_info);

    int i;

    while (true) {
        for(i = 0; i <= 360; i++) {
            servo_set(0, i);
            vTaskDelay(10 / portTICK_RATE_MS);
        }
        for(i = 360; i >= 0; i--) {
            servo_set(0, i);
            vTaskDelay(10 / portTICK_RATE_MS);
        }
    }
}
