//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "profile.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "ble/att_server.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 16
#define SCL_PIN 17
#define ADDR 0x44

// BAUD_RATE 100kHz
// 規格上は100kHzまでだが、実際は400kHzまで動作する
// standard mode 100kHz <- これで十分
// fast mode 400kHz
// fast mode plus 1MHz
// high speed mode 3.4MHz
#define BAUD_RATE 100000

uint16_t company_identifier = 0xFFFE;

static btstack_packet_callback_registration_t hci_event_callback_registration;
const uint8_t DISCVERABLE_MODE = 0x06;
uint8_t adv_data[] = {
   // Flags general discoverable
   0x02,
   BLUETOOTH_DATA_TYPE_FLAGS,
   DISCVERABLE_MODE,
   // Name
   0x0A,
   0x09,
   'L', 'E', ' ', 'b','e','a','c','o','n',
   0x00,// 0埋め
   0x00,// 0埋め
   0x00,// 0埋め
   0x00,// 0埋め
   0x00,// 0埋め
   0x00,// 0埋め
};

const uint8_t adv_data_len = sizeof(adv_data);

static btstack_timer_source_t timer;

void update_advertisement_data(void) {
    // 新しいアドバタイズメントデータを設定

    // SHT31-DIS データシート
    // https://strawberry-linux.com/pub/Sensirion_Humidity_SHT3x_DIS_Datasheet_V3_J.pdf
    uint8_t buf[2] = {0x2C, 0x06};
    i2c_write_blocking(I2C_PORT, ADDR, buf, sizeof(buf), false);

    sleep_ms(15);

    i2c_read_blocking(I2C_PORT, ADDR, buf, 6, false);

    uint16_t temperature = (buf[0] << 8) | buf[1];
    uint16_t humidity = (buf[3] << 8) | buf[4];


    float converted_temp = -45 + (175 * (temperature/ 65535.0));
    float converted_hum = 100 * (humidity / 65535.0);
    printf("Temperature: raw=%d, %f\n", temperature, converted_temp);
    printf("Humidity: raw=%d, %f\n", humidity, converted_hum);


    uint8_t humidity_data[] = {
      (uint8_t)((humidity & 0xFF00) >> 8),
      (uint8_t)(humidity & 0x00FF),
    };

    adv_data[14] = 0x04 + sizeof(humidity_data);
    adv_data[15] = 0xFF;
    adv_data[16] = (uint8_t)(company_identifier & 0x00FF);
    adv_data[17] = (uint8_t)((company_identifier & 0xFF00) >> 8);
    adv_data[18] = humidity_data[0];
    adv_data[19] = humidity_data[1];

    gap_advertisements_set_data(sizeof(adv_data), adv_data);

    printf("Advertisement data updated. %d %d\n", humidity_data[0], humidity_data[1]);
}

static void heartbeat_handler(btstack_timer_source_t *ts){
    // ここで定期的に実行したい処理を行う
    printf("ハートビート\n");
    update_advertisement_data();

    btstack_run_loop_set_timer(ts, 10000);
    btstack_run_loop_add_timer(ts);
}

static void le_advertise_setup(void){

    l2cap_init();

    att_server_init(profile_data, NULL, NULL);

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*)adv_data);

    gap_advertisements_enable(1);

}


int main(void)
{

    stdio_init_all();

    cyw43_arch_init();
    le_advertise_setup();

    // turn on!
    hci_power_control(HCI_POWER_ON);


    i2c_init(I2C_PORT, BAUD_RATE);

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);


    timer.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&timer, 10000); // 10000ミリ秒後に初回実行
    btstack_run_loop_add_timer(&timer);

    printf("初期化DONE\n");

    btstack_run_loop_execute();


    return 0;
}
