#pragma once

#include <inaddr.h>
#include <in6addr.h>

typedef struct tag_ST_IP_ADDRESSES
{
	IN_ADDR TunnelIpv4;
	IN_ADDR InternetIpv4;

	IN6_ADDR TunnelIpv6;
	IN6_ADDR InternetIpv6;
}
ST_IP_ADDRESSES;

namespace ip
{

bool
ValidTunnelIpv4Address
(
	const ST_IP_ADDRESSES *IpAddresses
);

bool
ValidInternetIpv4Address
(
	const ST_IP_ADDRESSES *IpAddresses
);

bool
ValidTunnelIpv6Address
(
	const ST_IP_ADDRESSES *IpAddresses
);

bool
ValidInternetIpv6Address
(
	const ST_IP_ADDRESSES *IpAddresses
);

} // namespace ip
