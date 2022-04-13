#pragma once

#include "ember/graphics/instance.hpp"

#include <ember/memory/unique_ptr.hpp>
#include <ember/core/export.hpp>

namespace ember::graphics {
    enum class api {
        vulkan
    };
}

namespace ember::graphics {
    /**
     * @brief Creates a new graphics instance that uses the provided API.
     * @param api 
     * @return 
     */
    EMBER_API memory::unique_ptr<instance> create_instance(api api);
}