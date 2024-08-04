#pragma once
#include "mhl/sys/fd.hpp"
#include <cstdint>
#include <format>
#include <optional>
#include <string>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

namespace mhl::sys::net
{
    class netlink_sock
    {
    public:
        explicit netlink_sock(const int flags)
            : _fd{::socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, flags)}
        {

        }

        [[nodiscard]] std::optional<std::string> bind_sock(sockaddr* sockaddr) noexcept;
        [[nodiscard]] std::optional<std::string> set_dev_ip(const std::string_view dev_name, const std::string_view ip_address) noexcept;
        [[nodiscard]] std::optional<std::string> set_link(const std::string_view dev_name,[[maybe_unused]] const bool state) noexcept;

        [[nodiscard]] inline mhl::sys::fd& get_fd() noexcept
        {
            return _fd;
        }

        [[nodiscard]] static sockaddr_nl create_sockaddr(const uint32_t flags) noexcept;

    private:
        mhl::sys::fd _fd{-1};
    };
}  // namespace mhl::sys::net
