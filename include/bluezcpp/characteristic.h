#pragma once
#include "bluezcpp/common.h"
#include "bluezcpp/descriptor.h"
#include <cstddef>
#include <dbuscpp/dbuscpp.h>
#include <vector>

namespace bluez {

class Characteristic {
public:
  Characteristic( dbus::ObjectPath path );
  Characteristic( const Characteristic &other );
  Characteristic &operator=( const Characteristic &rhs );

  UnixFD acquireNotify();
  UnixFD acquireWrite();
  std::vector<std::byte> readValue();
  void startNotify();
  void stopNotify();
  void writeValue( std::vector<std::byte> value );

  std::vector<Descriptor> descriptors();
  Descriptor descriptor( dbus::ObjectPath path );

  dbus::ObjectPath objectPath();

  // Properties
  std::vector<std::byte> value();
  std::vector<Flags> flags();
  bool notifyAcquired();
  bool notifying();
  bool writeAcquired();
  dbus::ObjectPath service();
  UUID uuid();

private:
  dbus::ObjectPath m_objectPath;
  std::shared_ptr<dbus::Manager> m_dbus;

};  // class Characteristic
}  // namespace bluez
