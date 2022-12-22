#include "bluezcpp/bluezcpp.h"
#include <iostream>
#include <string>

inline void wait_for( int seconds ) {
  std::this_thread::sleep_for( std::chrono::seconds( seconds ) );
}

// "680c21d9-c946-4c1f-9c11-baa1c21329e7"
using namespace bluez;
int main() {
  dbus::ObjectPath device_path( "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA" );
  dbus::ObjectPath srv_path( "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA/service000b" );
  dbus::ObjectPath char_path( "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA/service000b/char001e" );
}
