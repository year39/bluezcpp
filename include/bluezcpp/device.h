#pragma once
#include "bluezcpp/common.h"
#include "bluezcpp/service.h"
#include <dbuscpp/dbuscpp.h>
#include <vector>

namespace bluez {

class Device {
public:
  Device( Address address );
  Device( dbus::ObjectPath path );
  Device( const Device &other );
  Device &operator=( const Device &other );
  ~Device();
  void connect();
  void connect( std::function<void( Device )> callback );
  void connectProfile( UUID profile );
  void connectProfile( UUID profile, std::function<void( Device )> callback );

  void disconnect();
  void disconnectProfile( UUID profile );

  std::vector<Service> services();
  Service service( dbus::ObjectPath path );
  Service service( UUID uuid );
  dbus::ObjectPath objectPath();
  bool contains( UUID uuid );

  // Properties
  std::vector<UUID> uuids();
  bool blocked();
  bool connected();
  int16_t rssi();
  int16_t txPower();
  dbus::ObjectPath adapter();
  Address address();
  AddressType addressType();
  std::string alias();
  std::string alias( std::string newAlias );
  std::string name();
  std::string name( std::string newName );

private:
  dbus::ObjectPath m_objectPath;
  std::shared_ptr<dbus::Manager> m_dbus;
};
}  // namespace bluez
