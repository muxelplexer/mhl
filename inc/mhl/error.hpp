#pragma once
#include <optional>
#include <string>

namespace mhl::error
{
    using status_code = int;
    using error = std::optional<std::string>;

    void throw_err(const error& err);
}
