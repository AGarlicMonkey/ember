#pragma once

#include <ember/core/export.hpp>

namespace ember::platform {
    /**
     * @brief Pumps the platform's message queue. Dispatching any input / window messages.
     * Should be called every frame
     */
    EMBER_API void pump_messages();
}