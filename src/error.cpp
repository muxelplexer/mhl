#include "mhl/error.hpp"
#include <cstdlib>
#include <print>
#include <stdexcept>

namespace mhl::error
{
    void throw_err(const error& err)
    {
        if (!err.has_value()) return;
        throw std::runtime_error(err.value());
    }
}
