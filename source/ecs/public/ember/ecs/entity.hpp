#pragma once

#include <cinttypes>

namespace ember::ecs {
    using entity = std::uint32_t;

    inline entity constexpr null_entity{ 0 };
}