#pragma once
#include <dbuscpp/common.h>
#include <string>
#include <unistd.h>
#include <vector>

namespace bluez {

const std::string bluezBus { "org.bluez" };
const std::string bluezAdapter { "/org/bluez/hci0" };

enum class AddressType { UNKNOWN, PUBLIC, PRIVATE };

enum class Transport {
  AUTO,  //"auto"
  BR_EDR,  //"bredr"
  LE  //"le"
};

enum class Flags {
  BROADCAST,
  READ,
  WRITE_WITHOUT_RESPONSE,
  WRITE,
  NOTIFY,
  INDICATE,
  AUTHENTICATED_SIGNED_WRITES,
  EXTENDED_PROPERTIES,
  RELIABLE_WRITE,
  WRITABLE_AUXILIARIES,
  ENCRYPT_READ,
  ENCRYPT_WRITE,
  ENCRYPT_AUTHENTICATED_READ,
  ENCRYPT_AUTHENTICATED_WRITE,
  SECURE_READ,
  SECURE_WRITE,
  AUTHORIZE
};

class UUID : public std::string {
public:
  UUID() : std::string {} {}
  UUID( std::string uuid ) : std::string { uuid } {}
};

class Address : public std::string {
public:
  Address() : std::string {} {}
  Address( std::string address ) : std::string { address } {}
  dbus::ObjectPath toObjectPath() {
    std::string copy { data() };
    for ( auto& c : copy ) {
      c = ::toupper( c );
      if ( c == ':' )
        c = '_';
    }
    copy = bluezAdapter + "/dev_" + copy;
    return dbus::ObjectPath { copy };
  }
};

class UnixFD {
public:
  UnixFD() {}
  ~UnixFD() {
    close();
  }
  int get() {
    return fd;
  }
  void update( int newFD ) {
    if ( valid() )
      close();
    fd = newFD;
  }
  void close() {
    fd = ::close( fd );
  }
  bool valid() {
    return fd >= 0;
  }

private:
  int fd = -1;
};

}  // namespace bluez
