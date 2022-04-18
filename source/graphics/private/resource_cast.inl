#include <ember/core/log.hpp>

namespace ember::graphics {
    template<typename dest_type, typename source_type>
    dest_type *resource_cast(source_type *source) {
#if EMBER_GRAPHICS_USAGE_VALIDATION
        auto *const result{ dynamic_cast<dest_type *>(source) };
        EMBER_CHECK(result != nullptr);
        return result;
#else
        return static_cast<dest_type *>(source);
#endif
    }
}