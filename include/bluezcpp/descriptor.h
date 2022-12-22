#pragma once
#include "bluezcpp/common.h"
#include <cstddef>
#include <dbuscpp/dbuscpp.h>
#include <string>
#include <vector>

namespace bluez {

class Descriptor {
public:
  Descriptor( dbus::ObjectPath path );
  Descriptor( const Descriptor &other );
  Descriptor &operator=( const Descriptor &rhs );

  std::vector<std::byte> readValue();
  void writeValue( std::vector<std::byte> value );

  dbus::ObjectPath objectPath();

  // Properties
  std::vector<std::byte> value();
  dbus::ObjectPath characteristic();
  UUID uuid();

private:
  dbus::ObjectPath m_objectPath;
  std::shared_ptr<dbus::Manager> m_dbus;

};  // class Descriptor
}  // namespace bluez
