#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "profile.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "ble/att_server.h"

static btstack_packet_callback_registration_t hci_event_callback_registration;
const uint8_t DISCVERABLE_MODE = 0x06;
const uint8_t adv_data[] = {
   // Flags general discoverable
   0x02,
   BLUETOOTH_DATA_TYPE_FLAGS,
   DISCVERABLE_MODE,
   // Name
   0x0b,
   BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
   'L', 'E', ' ', 'b','e','a','c','o','n',
   // Incomplete List of 16-bit Service Class UUIDs -- FF10 - only valid for testing!
   0x03,
   BLUETOOTH_DATA_TYPE_INCOMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS,
   0x10,
   0xff,
};

const uint8_t adv_data_len = sizeof(adv_data);

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
    cyw43_arch_init();
    le_advertise_setup();

    // turn on!
	  hci_power_control(HCI_POWER_ON);

    btstack_run_loop_execute();
    return 0;
}
