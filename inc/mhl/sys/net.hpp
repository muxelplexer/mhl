#pragma once
#include "mhl/sys/fd.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iterator>
#include <string>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
namespace mhl
{
    namespace sys::net
    {
        template<class T>
        T hton(const T);

        template<class T>
        T ntoh(const T);

        using mac_address  = std::array<std::uint8_t, 6>;
        using ipv4_address = std::array<std::uint8_t, 8>;

        template<>
        [[nodiscard]] inline std::uint16_t hton(const std::uint16_t value)
        {
            return htons(value);
        }

        template<>
        [[nodiscard]] inline std::uint32_t hton(const std::uint32_t value)
        {
            return htonl(value);
        }

        template<>
        [[nodiscard]] inline std::uint16_t ntoh(const std::uint16_t value)
        {
            return ntohs(value);
        }

        template<>
        [[nodiscard]] inline std::uint32_t ntoh(const std::uint32_t value)
        {
            return ntohl(value);
        }

        [[nodiscard]] inline ssize_t send(mhl::sys::fd& fd, const std::string& data)
        {
            return ::send(static_cast<int>(fd), data.data(), data.length(), 0);
        }

        template<class T>
        [[nodiscard]] inline ssize_t send(mhl::sys::fd& fd, const T& data, const size_t data_len = sizeof(T))
        {
            return ::send(static_cast<int>(fd), &data, data_len, 0);
        }

    }  // namespace sys::net

    [[nodiscard]] inline std::string to_string(const sys::net::mac_address& addr) noexcept
    {
        return std::format("{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
                addr[0], addr[1], 
                addr[2], addr[2],
                addr[4], addr[5]);
    }

    [[nodiscard]] inline std::string to_string(const sys::net::ipv4_address& addr) noexcept
    {
        return std::format("{}.{}.{}.{}", addr[0], addr[1], addr[2], addr[3]);
    }
}  // namespace mhl
