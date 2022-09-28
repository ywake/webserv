#include "error.hpp"
#include "gtest.h"
#include "host_port.hpp"

using http_abnf::HostPort;

TEST(host_port_test, valid_host_port)
{
	EXPECT_EQ(HostPort("example.com:80"), HostPort("example.com", "80"));
	EXPECT_EQ(HostPort("example.com:"), HostPort("example.com", ""));

	EXPECT_EQ(HostPort("example.com"), HostPort("example.com", ""));
	EXPECT_EQ(HostPort("example.com80"), HostPort("example.com80", ""));

	EXPECT_EQ(HostPort(":"), HostPort("", ""));
	EXPECT_EQ(HostPort(":80"), HostPort("", "80"));
	EXPECT_EQ(HostPort(""), HostPort("", ""));

}

TEST(host_port_test, invalid_host_port)
{
	EXPECT_THROW(HostPort(":port"), Error);
	EXPECT_THROW(HostPort("a@a:80"), Error);
}
