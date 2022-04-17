#pragma once

namespace ember::graphics {
    enum class sharing_mode {
        exclusive, /**< Can only be accessed by a single queue type at a time. The ownership must be explicitly transfered between queues. */
        concurrent /**< Can be accessed between different queue types without transferring ownership. */
    };

    enum class memory_type {
        host_visible, /**< Can be written to by the host, not device optimised. */
        device_local, /**< Can't be written to by the host, device optimised. */
    };
}