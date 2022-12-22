#pragma once
#include "bluezcpp/characteristic.h"
#include "bluezcpp/common.h"
#include <dbuscpp/dbuscpp.h>
#include <vector>

namespace bluez {

class Service {
public:
  Service( dbus::ObjectPath path );
  Service( const Service &s );
  Service &operator=( const Service &rhs );

  std::vector<Characteristic> characteristics();
  Characteristic characteristic( dbus::ObjectPath path );
  Characteristic characteristic( UUID uuid );

  dbus::ObjectPath objectPath();

  // Properties
  std::vector<dbus::ObjectPath> inclues();
  bool primary();
  dbus::ObjectPath device();
  UUID uuid();

private:
  dbus::ObjectPath m_objectPath;
  std::shared_ptr<dbus::Manager> m_dbus;

};  // class Service
}  // namespace bluez
