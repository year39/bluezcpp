#pragma once
#include <dbuscpp/dbuscpp.h>
#include <stdexcept>
#include <string>

namespace bluez {

inline void THROW_EXCEPTION_IF( bool condition, std::string message ) {
  if ( condition )
    throw std::runtime_error( message );
}

class DBusManager {
public:
  DBusManager( const DBusManager& ) = delete;
  DBusManager& operator=( DBusManager& ) = delete;
  static DBusManager& instance() {
    static DBusManager obj;
    return obj;
  }

  std::shared_ptr<dbus::Manager> get() {
    return dbus;
  }

private:
  DBusManager() {
    dbus = std::make_shared<dbus::Manager>();
  }

  std::shared_ptr<dbus::Manager> dbus;
};
}  // namespace bluez
