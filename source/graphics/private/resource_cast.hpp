#pragma once

namespace ember::graphics {
    template<typename dest_type, typename source_type>
    dest_type *resource_cast(source_type *source);
}

#include "resource_cast.inl"