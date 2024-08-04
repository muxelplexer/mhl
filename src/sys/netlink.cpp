#include "mhl/sys/netlink.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <format>
#include <linux/if_addr.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <optional>
#include <string>

#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

#include "mhl/sys/net.hpp"

namespace mhl::sys::net
{
    std::optional<std::string> netlink_sock::bind_sock(sockaddr* sockaddr) noexcept
    {
        if (::bind(static_cast<int>(_fd), sockaddr, sizeof(*sockaddr)) < 0)
        {
            return std::format("Failed to bind netlink socket.");
        }
        return std::nullopt;
    }
    std::optional<std::string> netlink_sock::set_dev_ip(const std::string_view dev_name, const std::string_view ip_addr) noexcept
    {
        struct req_type {
            struct nlmsghdr  header;
            struct ifaddrmsg content;
            std::array<uint8_t, 64> attr_buf;
        } request = { };
        struct rtattr* request_attr;
        size_t attributes_buf_avail = request.attr_buf.max_size();

        request.header.nlmsg_len = NLMSG_LENGTH(sizeof(request.content));
        request.header.nlmsg_flags = NLM_F_REQUEST | NLM_F_EXCL | NLM_F_CREATE;
        request.header.nlmsg_type = RTM_NEWADDR;
        request.content.ifa_index = if_nametoindex(dev_name.data());
        request.content.ifa_family = AF_INET;
        request.content.ifa_prefixlen = 24;

        request_attr = IFA_RTA(&request.content);
        request_attr->rta_type = IFA_LOCAL;
        request_attr->rta_len = RTA_LENGTH(sizeof(struct in_addr));
        request.header.nlmsg_len += request_attr->rta_len;
        inet_pton(AF_INET, ip_addr.data(), RTA_DATA(request_attr));

        request.header.nlmsg_len += request_attr->rta_len;
        request_attr = RTA_NEXT(request_attr, attributes_buf_avail);
        request_attr->rta_type = IFA_ADDRESS;
        request_attr->rta_len = RTA_LENGTH(sizeof(struct in_addr));
        inet_pton(AF_INET, ip_addr.data(), RTA_DATA(request_attr));

        if(auto send_bytes = mhl::sys::net::send<req_type>(_fd, request); send_bytes < 0)
        {
            return std::format("Could not set ip address. (addr: {})", ip_addr);
        }

        return std::nullopt;
    }

    std::optional<std::string> netlink_sock::set_link(const std::string_view dev_name,[[maybe_unused]] const bool state) noexcept
    {
        struct req_type {
            struct nlmsghdr  header;
            struct ifinfomsg content;
        } request = { };

        request.header.nlmsg_len = NLMSG_LENGTH(sizeof request.content);
        request.header.nlmsg_flags = NLM_F_REQUEST;
        request.header.nlmsg_type = RTM_NEWLINK;
        request.content.ifi_index = static_cast<int>(if_nametoindex(dev_name.data()));

        // TODO: find out how to explicitly up/down an interface
        request.content.ifi_flags = IFF_UP;
        request.content.ifi_change = 1;
        
        if(auto send_bytes = mhl::sys::net::send<req_type>(_fd, request); send_bytes < 0)
        {
            return std::format("Could not set link");
        }

        return std::nullopt;
    }

    sockaddr_nl netlink_sock::create_sockaddr(const uint32_t flags) noexcept 
    {
        return {
            .nl_family = AF_NETLINK,
            .nl_pad = 0,
            .nl_pid = static_cast<uint32_t>(::getpid()),
            .nl_groups = flags
        };
    }
}  // namespace mhl::sys::net
