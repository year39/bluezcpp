#include "bluezcpp/service.h"
#include "internal.h"
#include <regex>

using namespace bluez;

Service::Service( dbus::ObjectPath path ) : m_objectPath { path } {
  m_dbus = DBusManager::instance().get();
}

Service::Service( const Service &other ) {
  m_objectPath = other.m_objectPath;
  m_dbus = other.m_dbus;
}

Service &Service::operator=( const Service &rhs ) {
  m_objectPath = rhs.m_objectPath;
  return *this;
}

std::vector<Characteristic> Service::characteristics() {
  std::vector<Characteristic> chars;
  std::vector<dbus::ObjectPath> objects = m_dbus->objects( bluezBus );

  std::string s;
  std::regex regex( m_objectPath + "/char[0-9a-z]+$" );
  for ( auto i : objects ) {
    if ( std::regex_search( i, regex ) ) {
      chars.push_back( Characteristic { i } );
    }
  }
  return chars;
}

Characteristic Service::characteristic( dbus::ObjectPath path ) {
  return Characteristic { path };
}

dbus::ObjectPath Service::objectPath() {
  return m_objectPath;
}

Characteristic Service::characteristic( UUID uuid ) {
  std::vector<Characteristic> c = characteristics();
  for ( auto j : c ) {
    if ( j.uuid() == uuid )
      return j;
  }
  std::string what { "Characteristic UUID {" + uuid + "} could not be found" };
  throw std::runtime_error( what );
}

std::vector<dbus::ObjectPath> Service::inclues() {
  std::vector<dbus::ObjectPath> objects;
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.GattService1", "Includes" );
  dbus::Reply reply = m_dbus->call( m );
  reply.read( objects );
  return objects;
}

bool Service::primary() {
  bool isPrimary = false;
  m_dbus->propertyGetDirect(
    bluezBus, m_objectPath, "org.bluez.GattService1", "Primary", isPrimary );
  return isPrimary;
}

dbus::ObjectPath Service::device() {
  dbus::ObjectPath p;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.GattService1", "Device", p );
  return p;
}

UUID Service::uuid() {
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.GattService1", "UUID", s );
  return UUID { s };
}
