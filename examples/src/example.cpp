#include "bluezcpp/bluezcpp.h"
#include <iostream>
#include <string>

inline void wait_for( int seconds ) {
  std::this_thread::sleep_for( std::chrono::seconds( seconds ) );
}

// "680c21d9-c946-4c1f-9c11-baa1c21329e7"
using namespace bluez;
int main() {
  dbus::ObjectPath dop( "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA" );
  dbus::ObjectPath sop( "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA/service000b" );
  dbus::ObjectPath cop( "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA/service000b/char000f" );

  Adapter adapter;
  std::cout << "adapter object: " << adapter.objectPath() << "\n";
  std::cout << "adapter name: " << adapter.name() << "\n";
  std::cout << "adapter address: " << adapter.address() << "\n";
  std::cout << "address type: ";
  switch ( adapter.addressType() ) {
    case bluez::AddressType::UNKNOWN:
      std::cout << "Unknonw";
      break;
    case bluez::AddressType::PUBLIC:
      std::cout << "PUBLIC";
      break;
    case bluez::AddressType::PRIVATE:
      std::cout << "PRIVATE";
      break;
  }

#if 0
  std::cout << "\n";
  std::cout << "uuids: \n";
  std::vector<UUID> uuids = adapter.uuids();
  for ( auto &i : uuids ) {
    std::cout << "-- " << i << "\n";
  }
  std::cout << "--------------------------------------------------\n\n";

  std::cout << "start discovery...\n";
  adapter.removeAllDevices();
  adapter.startDiscovery();
  wait_for( 5 );
  adapter.stopDiscovery();
  std::cout << "discovery ended\n\n";

  auto objectPaths = adapter.deviceObjectPaths();
  for ( auto objectPath : objectPaths ) {
    Device device { objectPath };
    try {
      std::cout << "device object: " << device.objectPath() << "\n";
      std::cout << "device adapter: " << device.adapter() << "\n";
      std::cout << "device address: " << device.address() << "\n";
      std::cout << "device name/alias: " << device.name() << "/" << device.alias() << "\n";
      std::cout << "device connected: " << ( device.connected() ? "true" : "false" ) << "\n";
      std::cout << "device rssi/txPower: " << device.rssi() << "/" << device.txPower() << "\n";
    } catch ( std::runtime_error &e ) {
      std::cout << "access failed: " << e.what() << "\n";
    }
    std::cout << "device uuids: ";
    std::vector<UUID> dev_uuids = device.uuids();
    for ( auto &uuid : dev_uuids ) {
      std::cout << "-- " << uuid << "\n";
    }
    std::cout << "\n";
  }
#endif

  std::cout << "\n";
  std::cout << "--------------------------------------------------\n\n";
  Device device { Address { "Ee:CB:5E:62:B3:DA" } };
  std::cout << device.objectPath() << "\n";
  device.connect();
  Service srv = device.service( UUID { "680c21d9-c946-4c1f-9c11-baa1c21329e7" } );
  std::cout << "service object: " << srv.objectPath() << "\n";
  std::cout << "service device: " << srv.device() << "\n";
  std::cout << "service uuid: " << srv.uuid() << "\n";
  std::cout << "service primary: " << ( srv.primary() ? "true" : "false" ) << "\n";
  std::cout << "--------------------------------------------------\n\n";

  Characteristic c = srv.characteristic( UUID { "003bbdf2-c634-4b3d-ab56-7ec889b89a37" } );
  std::cout << "char object: " << c.objectPath() << "\n";
  std::cout << "char service: " << c.service() << "\n";
  std::cout << "char uuid: " << c.uuid() << "\n";
  std::cout << "value: ";
  std::vector<std::byte> v = c.value();
  for ( auto &i : v ) {
    std::cout << +static_cast<uint8_t>( i ) << " ";
  }
  std::cout << "Descriptors: \n";
  std::vector<Descriptor> descriptors = c.descriptors();
  for ( auto &i : descriptors ) {
    std::cout << "-- " << i.objectPath() << "\n";
  }
  std::cout << "\n";
  std::cout << "--------------------------------------------------\n\n";

  Descriptor desc = c.descriptor(
    dbus::ObjectPath { "/org/bluez/hci0/dev_EE_CB_5E_62_B3_DA/service000b/char000f/desc0011" } );
  std::cout << "desc object: " << desc.objectPath() << "\n";
  std::cout << "desc char:" << desc.characteristic() << "\n";
  std::cout << "desc uuid: " << desc.uuid() << "\n";
  std::cout << "\n";
  std::cout << "--------------------------------------------------\n\n";

  std::cout << "connecting to device...\n";

  std::cout << "char readValue: ";
  std::vector<std::byte> value = c.readValue();
  for ( auto &v : value ) {
    std::cout << +static_cast<uint8_t>( v ) << " ";
  }
  std::cout << std::endl;

  std::cout << "start notify...\n";
  c.startNotify();

  auto sig = dbus::SignalGroup::createSignal();
  std::string rule = "type=signal,path=" + c.objectPath();
  dbus::SignalGroup::matchRule( sig, rule );
  std::cout << rule << "\n";
  dbus::SignalGroup::signalCallback( sig, [&]( dbus::SignalID id ) {
    std::cout << id << "\n";
    auto value = c.readValue();
    for ( auto &v : value ) {
      std::cout << +static_cast<uint8_t>( v ) << " ";
    }
    std::cout << std::endl;
  } );
  std::cout << sig << "\n";
  dbus::SignalGroup::add( sig );
  dbus::SignalGroup::start();
  wait_for( 10 );
  dbus::SignalGroup::remove( sig );
  std::cout << "stop notify\n";
  c.stopNotify();
  std::cout << "disconnecting...\n";
  dbus::SignalGroup::reset();
  device.disconnect();
}
