#include "bluezcpp/device.h"
#include "bluezcpp/adapter.h"
#include "internal.h"
#include <regex>

using namespace bluez;

Device::Device( Address address ) {
  m_objectPath = address.toObjectPath();
  m_dbus = DBusManager::instance().get();
}

Device::Device( dbus::ObjectPath path ) : m_objectPath( path ) {
  m_dbus = DBusManager::instance().get();
}

Device::Device( const Device &other ) {
  m_objectPath = other.m_objectPath;
  m_dbus = other.m_dbus;
}

Device::~Device() {}

Device &Device::operator=( const Device &rhs ) {
  m_objectPath = rhs.m_objectPath;
  return *this;
}

void Device::connect() {
  dbus::Message m = m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Device1", "Connect" );
  m_dbus->call( m );
}

void Device::connectProfile( UUID profile ) {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Device1", "ConnectProfile" );
  m.write( profile );
  m_dbus->call( m );
}

void Device::disconnect() {
  dbus::Message m = m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Device1", "Disconnect" );
  m_dbus->call( m );
}

void Device::disconnectProfile( UUID profile ) {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Device1", "DisconnectProfile" );
  m.write( profile );
  m_dbus->call( m );
}

std::vector<Service> Device::services() {
  std::vector<Service> srv;
  std::vector<dbus::ObjectPath> objects = m_dbus->objects( bluezBus );

  std::string s;
  std::regex regex( m_objectPath + "/service[0-9a-z]+$" );
  for ( auto i : objects ) {
    if ( std::regex_search( i, regex ) ) {
      srv.push_back( Service { i } );
    }
  }
  return srv;
}

Service Device::service( dbus::ObjectPath path ) {
  return Service { path };
}

Service Device::service( UUID uuid ) {
  std::vector<Service> s = services();
  for ( auto j : s ) {
    if ( j.uuid() == uuid )
      return j;
  }

  std::string what { "Service UUID {" + uuid + "} could not be found" };
  throw std::runtime_error( what );
}

dbus::ObjectPath Device::objectPath() {
  return m_objectPath;
}

bool Device::contains( UUID uuid ) {
  bool found = false;
  std::vector<UUID> list = uuids();
  for ( auto i : list ) {
    if ( i == uuid )
      found = true;
  }
  return found;
}

std::vector<UUID> Device::uuids() {
  dbus::Reply reply = m_dbus->propertyGet( bluezBus, m_objectPath, "org.bluez.Device1", "UUIDs" );
  std::vector<std::string> uuids_string;
  std::vector<UUID> uuids_return;

  reply.enterContainer( dbus::DATA_TYPE::VARIANT, "as" );
  reply.read( uuids_string );
  reply.exitContainer();

  for ( auto i : uuids_string ) {
    uuids_return.push_back( UUID( i ) );
  }
  return uuids_return;
}

bool Device::blocked() {
  bool t = false;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Blocked", t );
  return t;
}

bool Device::connected() {
  bool t = false;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Connected", t );
  return t;
}

int16_t Device::rssi() {
  int16_t t = 0;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "RSSI", t );
  return t;
}

int16_t Device::txPower() {
  int16_t t;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "TxPower", t );
  return t;
}

dbus::ObjectPath Device::adapter() {
  dbus::ObjectPath t;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Adapter", t );
  return t;
}

Address Device::address() {
  std::string t;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Address", t );
  return Address { t };
}

AddressType Device::addressType() {
  AddressType t;
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "AddressType", s );

  if ( s == "public" )
    t = AddressType::PUBLIC;
  else if ( s == "private" )
    t = AddressType::PRIVATE;
  else
    t = AddressType::UNKNOWN;

  return t;
}

std::string Device::alias() {
  std::string t = "";
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Alias", t );
  return t;
}

std::string Device::alias( std::string newAlias ) {
  m_dbus->propertySetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Alias", newAlias );
  return alias();
}

std::string Device::name() {
  std::string t = "";
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Name", t );
  return t;
}

std::string Device::name( std::string newName ) {
  m_dbus->propertySetDirect( bluezBus, m_objectPath, "org.bluez.Device1", "Name", newName );
  return name();
}
