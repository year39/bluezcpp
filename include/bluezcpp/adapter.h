#pragma once
#include "bluezcpp/common.h"
#include "bluezcpp/device.h"
#include <dbuscpp/dbuscpp.h>
#include <string>
#include <vector>

namespace bluez {

class Adapter {
public:
  Adapter();
  Adapter( const Adapter& ) = delete;
  Adapter& operator=( const Adapter& ) = delete;

  void setDiscoveryFilter( Transport transport );
  void setDiscoveryFilter( int16_t rssi );
  void removeDiscoveryFilter();
  void startDiscovery();
  void stopDiscovery();
  std::vector<dbus::ObjectPath> deviceObjectPaths();
  dbus::ObjectPath objectPath();
  void hci( std::string hcix );

  // Properties
  std::vector<UUID> uuids();
  bool discoverable();
  bool discoverable( bool newStatus );
  bool discovering();
  bool powered();
  bool powered( bool newStatus );
  std::string alias();
  std::string alias( std::string newAlias );
  std::string name();
  Address address();
  AddressType addressType();

  void removeDevice( dbus::ObjectPath path );
  void removeAllDevices();

private:
  dbus::ObjectPath m_objectPath;
  std::shared_ptr<dbus::Manager> m_dbus;

};  // class Adapter
}  // namespace bluez
