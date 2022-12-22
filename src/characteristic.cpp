#include "bluezcpp/characteristic.h"
#include "internal.h"
#include <regex>

using namespace bluez;

Characteristic::Characteristic( dbus::ObjectPath path ) : m_objectPath { path } {
  THROW_EXCEPTION_IF( path.empty(), "" );
  m_dbus = DBusManager::instance().get();
}

Characteristic::Characteristic( const Characteristic &other ) {
  m_objectPath = other.m_objectPath;
  m_dbus = other.m_dbus;
}

Characteristic &Characteristic::operator=( const Characteristic &rhs ) {
  m_objectPath = rhs.m_objectPath;
  return *this;
}

UnixFD Characteristic::acquireNotify() {
  return UnixFD {};
}

UnixFD Characteristic::acquireWrite() {
  return UnixFD {};
}

std::vector<std::byte> Characteristic::readValue() {
  std::vector<std::byte> v;
  std::string s = "offset";
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "ReadValue" );
  // ReadValue takes a{sv} argument that we are not interested in
  // its use. So we are adding an empty array as an argument.
  m.openContainer( dbus::DATA_TYPE::ARRAY, "{sv}" );
  m.closeContainer();
  dbus::Reply reply = m_dbus->call( m );
  reply.read( v );
  return v;
}

void Characteristic::startNotify() {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "StartNotify" );
  m_dbus->call( m );
}

void Characteristic::stopNotify() {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "StopNotify" );
  m_dbus->call( m );
}

void Characteristic::writeValue( std::vector<std::byte> value ) {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "WriteValue" );
  m.write( value );
  m.openContainer( dbus::DATA_TYPE::ARRAY, "{sv}" );
  m.closeContainer();
  m_dbus->call( m );
}

std::vector<Descriptor> Characteristic::descriptors() {
  std::vector<Descriptor> v;
  std::vector<dbus::ObjectPath> objects = m_dbus->objects( bluezBus );

  std::string s;
  std::regex regex( m_objectPath + "/desc[0-9a-z]+$" );
  for ( auto i : objects ) {
    if ( std::regex_search( i, regex ) ) {
      v.push_back( Descriptor { i } );
    }
  }
  return v;
}

Descriptor Characteristic::descriptor( dbus::ObjectPath path ) {
  return Descriptor { path };
}

dbus::ObjectPath Characteristic::objectPath() {
  return m_objectPath;
}

std::vector<std::byte> Characteristic::value() {
  std::vector<std::byte> v;
  dbus::Reply reply =
    m_dbus->propertyGet( bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "Value" );
  reply.enterContainer( dbus::DATA_TYPE::VARIANT, "ay" );
  reply.read( v );
  reply.exitContainer();
  return v;
}

dbus::ObjectPath Characteristic::service() {
  dbus::ObjectPath p;
  m_dbus->propertyGetDirect(
    bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "Service", p );
  return p;
}

UUID Characteristic::uuid() {
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.GattCharacteristic1", "UUID", s );
  return UUID { s };
}
