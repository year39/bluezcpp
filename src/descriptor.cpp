#include "bluezcpp/descriptor.h"
#include "internal.h"

using namespace bluez;

Descriptor::Descriptor( dbus::ObjectPath path ) : m_objectPath { path } {
  m_dbus = DBusManager::instance().get();
}

Descriptor::Descriptor( const Descriptor &other ) {
  m_objectPath = other.m_objectPath;
}

Descriptor &Descriptor::operator=( const Descriptor &rhs ) {
  m_objectPath = rhs.m_objectPath;
  return *this;
}

std::vector<std::byte> Descriptor::readValue() {
  std::vector<std::byte> v;
  dbus::Message m = m_dbus->methodCall(
    bluezBus, m_objectPath, "org.bluez.GattDescriptor1", "ReadValue" );
  dbus::Reply reply = m_dbus->call( m );
  reply.read( v );
  return v;
}

void Descriptor::writeValue( std::vector<std::byte> value ) {
  dbus::Message m = m_dbus->methodCall(
    bluezBus, m_objectPath, "org.bluez.GattDescriptor1", "WriteValue" );
  m.write( value );

  m.openContainer( dbus::DATA_TYPE::ARRAY, "{sv}" );
  m.closeContainer();

  m_dbus->call( m );
}

dbus::ObjectPath Descriptor::objectPath() {
  return m_objectPath;
}

std::vector<std::byte> Descriptor::value() {
  std::vector<std::byte> v;
  dbus::Reply reply =
    m_dbus->propertyGet( bluezBus, m_objectPath, "org.bluez.GattDescriptor1", "Value" );
  reply.read( v );
  return v;
}

dbus::ObjectPath Descriptor::characteristic() {
  dbus::ObjectPath p;
  m_dbus->propertyGetDirect(
    bluezBus, m_objectPath, "org.bluez.GattDescriptor1", "Characteristic", p );
  return p;
}

UUID Descriptor::uuid() {
  std::string s;
  m_dbus->propertyGetDirect(
    bluezBus, m_objectPath, "org.bluez.GattDescriptor1", "UUID", s );
  return UUID { s };
}
