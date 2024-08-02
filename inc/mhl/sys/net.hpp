#pragma once
#include <array>
#include <cstdint>
#include <format>
#include <string>

#include <netinet/in.h>
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

    }
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
}
