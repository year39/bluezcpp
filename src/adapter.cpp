#include "bluezcpp/adapter.h"
#include "internal.h"
#include <regex>

using namespace bluez;

Adapter::Adapter() : m_objectPath { bluezAdapter } {
  m_dbus = DBusManager::instance().get();
}

void Adapter::setDiscoveryFilter( Transport transport ) {
  // dbus message content a{sv}
  std::string transport_value;
  std::string transport_key = "Transport";

  switch ( transport ) {
    case Transport::AUTO:
      transport_value = "auto";
      break;
    case Transport::BR_EDR:
      transport_value = "bredr";
      break;
    case Transport::LE:
      transport_value = "le";
      break;
    default:
      transport_value = "";
  }

  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Adapter1", "SetDiscoveryFilter" );

  m.openContainer( dbus::DATA_TYPE::ARRAY, "{sv}" );

  m.openContainer( dbus::DATA_TYPE::DICT_ENTRY, "sv" );
  m.write( transport_key );
  m.openContainer( dbus::DATA_TYPE::VARIANT, dbus::DATA_TYPE::STRING );
  m.write( transport_value );
  m.closeContainer();
  m.closeContainer();

  m.closeContainer();
  dbus::Reply reply = m_dbus->call( m );
}

void Adapter::setDiscoveryFilter( int16_t rssi ) {
  // a{sv}
  std::string rssi_key = "RSSI";
  int16_t rssi_value = rssi;

  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Adapter1", "SetDiscoveryFilter" );

  m.openContainer( dbus::DATA_TYPE::ARRAY, "{sv}" );

  if ( rssi <= 0 && rssi >= -120 ) {
    m.openContainer( dbus::DATA_TYPE::DICT_ENTRY, "sv" );
    m.write( rssi_key );
    m.openContainer( dbus::DATA_TYPE::VARIANT, dbus::DATA_TYPE::INT16 );
    m.write( rssi_value );
    m.closeContainer();
    m.closeContainer();
  }

  m.closeContainer();
  dbus::Reply reply = m_dbus->call( m );
}

void Adapter::removeDiscoveryFilter() {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Adapter1", "SetDiscoveryFilter" );
  m.openContainer( dbus::DATA_TYPE::ARRAY, "{sv}" );
  m.closeContainer();
  m_dbus->call( m );
}

void Adapter::startDiscovery() {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Adapter1", "StartDiscovery" );
  m_dbus->call( m );
}

void Adapter::stopDiscovery() {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Adapter1", "StopDiscovery" );
  m_dbus->call( m );
}

std::vector<dbus::ObjectPath> Adapter::deviceObjectPaths() {
  std::vector<dbus::ObjectPath> objects = m_dbus->objects( bluezBus );
  std::vector<dbus::ObjectPath> devs;

  // example device: /org/bluez/hci0/dev_EE_CB_5E_62_B3_DA
  std::regex regex( m_objectPath + "/dev[0-9A-Z_]+$" );
  for ( auto i : objects ) {
    if ( std::regex_search( i, regex ) ) {
      devs.push_back( i );
    }
  }
  return devs;
}

dbus::ObjectPath Adapter::objectPath() {
  return m_objectPath;
}

void Adapter::hci( std::string hcix ) {
  m_objectPath = "/org/bluez/" + hcix;
}

std::vector<UUID> Adapter::uuids() {
  std::vector<std::string> uuids_string;
  std::vector<UUID> uuids;

  dbus::Reply reply = m_dbus->propertyGet( bluezBus, m_objectPath, "org.bluez.Adapter1", "UUIDs" );
  reply.enterContainer( dbus::DATA_TYPE::VARIANT, "as" );
  reply.read( uuids_string );
  reply.exitContainer();

  UUID uuid;
  for ( auto i : uuids_string ) {
    uuid = std::string { i };
    uuids.push_back( uuid );
  }
  return uuids;
}

bool Adapter::discoverable() {
  bool i = false;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Discoverable", i );
  return i;
}

bool Adapter::discoverable( bool newStatus ) {
  m_dbus->propertySetDirect(
    bluezBus, m_objectPath, "org.bluez.Adapter1", "Discoverable", newStatus );
  return discoverable();
}

bool Adapter::discovering() {
  bool i = false;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Discovering", i );
  return i;
}

bool Adapter::powered() {
  bool i = false;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Powered", i );
  return i;
}

bool Adapter::powered( bool newStatus ) {
  m_dbus->propertySetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Powered", newStatus );
  return powered();
}

std::string Adapter::alias() {
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Alias", s );
  return s;
}

std::string Adapter::alias( std::string newAlias ) {
  m_dbus->propertySetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Alias", newAlias );
  return alias();
}

std::string Adapter::name() {
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Name", s );
  return s;
}

Address Adapter::address() {
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "Address", s );
  return Address { s };
}

AddressType Adapter::addressType() {
  std::string s;
  m_dbus->propertyGetDirect( bluezBus, m_objectPath, "org.bluez.Adapter1", "AddressType", s );
  if ( s == "public" )
    return AddressType::PUBLIC;
  if ( s == "private" )
    return AddressType::PRIVATE;
  return AddressType::UNKNOWN;
}

void Adapter::removeDevice( dbus::ObjectPath path ) {
  dbus::Message m =
    m_dbus->methodCall( bluezBus, m_objectPath, "org.bluez.Adapter1", "RemoveDevice" );
  m.write( path );
  m_dbus->call( m );
}

void Adapter::removeAllDevices() {
  std::vector<dbus::ObjectPath> objects = m_dbus->objects( bluezBus );
  std::regex regex( m_objectPath + "/dev_.*$" );
  for ( auto &obj : objects ) {
    if ( std::regex_search( obj, regex ) ) {
      try {
        removeDevice( obj );
      } catch ( std::runtime_error &e ) {
        continue;
      }
    }
  }
}
