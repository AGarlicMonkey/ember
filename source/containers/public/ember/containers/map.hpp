#pragma once

#include <unordered_map>

namespace ember::inline containers {
    template<typename key_type, typename value_type>
    using map = std::unordered_map<key_type, value_type>;
}