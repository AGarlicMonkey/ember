#pragma once

#include <map>

namespace ember::inline containers {
    template<typename key_type, typename value_type>
    using ordered_map = std::map<key_type, value_type>;
}