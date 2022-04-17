#pragma once

#include <cinttypes>

namespace ember::graphics {
    struct queue_family_indices {
        std::uint32_t graphics{ -1u };//Will act as both the graphics and present queue
        std::uint32_t compute{ -1u };
        std::uint32_t transfer{ -1u };
    };
}