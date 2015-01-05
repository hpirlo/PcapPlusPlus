#define LOG_MODULE CommonLogModuleIpUtils

#include <Logger.h>
#include <IpAddress.h>
#include <IpUtils.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

IPAddress::~IPAddress()
{

}

auto_ptr<IPAddress> IPAddress::fromString(char* addressAsString)
{
	in_addr ip4Addr;
	in6_addr ip6Addr;
    if (inet_pton(AF_INET, addressAsString, &ip4Addr) != 0)
    {
    	return auto_ptr<IPAddress>(new IPv4Address(addressAsString));
    }
    else if (inet_pton(AF_INET6, addressAsString, &ip6Addr) != 0)
    {
    	return auto_ptr<IPAddress>(new IPv6Address(addressAsString));
    }

    return auto_ptr<IPAddress>(NULL);
}

auto_ptr<IPAddress> IPAddress::fromString(string addressAsString)
{
	return fromString((char*)addressAsString.c_str());
}

IPv4Address::IPv4Address(const IPv4Address& other)
{
	m_pInAddr = new in_addr();
	memcpy(m_pInAddr, other.m_pInAddr, sizeof(in_addr));

    strncpy(m_AddressAsString, other.m_AddressAsString, 40);
    m_IsValid = true;
}

IPv4Address::IPv4Address(uint32_t addressAsInt)
{
	m_pInAddr = new in_addr();
	memcpy(m_pInAddr, &addressAsInt, sizeof(addressAsInt));
	if (inet_ntop(AF_INET, m_pInAddr, m_AddressAsString, MAX_ADDR_STRING_LEN) == 0)
		m_IsValid = false;
	m_IsValid = true;
}

IPv4Address::IPv4Address(in_addr* inAddr)
{
	m_pInAddr = new in_addr();
	memcpy(m_pInAddr, inAddr, sizeof(in_addr));
	if (inet_ntop(AF_INET, m_pInAddr, m_AddressAsString, MAX_ADDR_STRING_LEN) == 0)
		m_IsValid = false;
	m_IsValid = true;
}

void IPv4Address::init(char* addressAsString)
{
	m_pInAddr = new in_addr();
    if (inet_pton(AF_INET, addressAsString , m_pInAddr) == 0)
    {
    	m_IsValid = false;
    	return;
    }

    strncpy(m_AddressAsString, addressAsString, 40);
    m_IsValid = true;
}

IPv4Address::~IPv4Address()
{
	delete m_pInAddr;
}

IPv4Address::IPv4Address(char* addressAsString)
{
	init(addressAsString);
}

IPv4Address::IPv4Address(string addressAsString)
{
	init((char*)addressAsString.c_str());
}

uint32_t IPv4Address::toInt() const
{
	uint32_t result;
	memcpy(&result, m_pInAddr, sizeof(uint32_t));
	return result;
}

bool IPv4Address::matchSubnet(const IPv4Address& subnet, const string& subnetMask)
{
	IPv4Address maskAsIpAddr(subnetMask);
	if (!maskAsIpAddr.isValid())
	{
		LOG_ERROR("Subnet mask '%s' is in illegal format", subnetMask.c_str());
		return false;
	}

	int thisAddrAfterMask = toInt() & maskAsIpAddr.toInt();
	int subnetAddrAfterMask = subnet.toInt() & maskAsIpAddr.toInt();
	return (thisAddrAfterMask == subnetAddrAfterMask);
}

IPv6Address::IPv6Address(const IPv6Address& other)
{
	m_pInAddr = new in6_addr();
	memcpy(m_pInAddr, other.m_pInAddr, sizeof(in_addr));

    strncpy(m_AddressAsString, other.m_AddressAsString, 40);
    m_IsValid = true;
}

IPv6Address::~IPv6Address()
{
	delete m_pInAddr;
}

void IPv6Address::init(char* addressAsString)
{
	m_pInAddr = new in6_addr();
    if (inet_pton(AF_INET6, addressAsString , m_pInAddr) == 0)
    {
    	m_IsValid = false;
    	return;
    }

    strncpy(m_AddressAsString, addressAsString, 40);
    m_IsValid = true;
}

IPv6Address::IPv6Address(uint8_t* addressAsUintArr)
{
	m_pInAddr = new in6_addr();
	memcpy(m_pInAddr, addressAsUintArr, 16);
	if (inet_ntop(AF_INET6, m_pInAddr, m_AddressAsString, MAX_ADDR_STRING_LEN) == 0)
		m_IsValid = false;
	m_IsValid = true;
}

IPv6Address::IPv6Address(char* addressAsString)
{
	init(addressAsString);
}

IPv6Address::IPv6Address(string addressAsString)
{
	init((char*)addressAsString.c_str());
}

void IPv6Address::copyTo(uint8_t** arr, size_t& length)
{
	length = 16;
	(*arr) = new uint8_t[length];
	memcpy((*arr), m_pInAddr, length);
}

void IPv6Address::copyTo(uint8_t* arr) const
{
	memcpy(arr, m_pInAddr, 16);
}

bool IPv6Address::operator==(const IPv6Address& other)
{
	return (memcmp(m_pInAddr, other.m_pInAddr, 16) == 0);
}
