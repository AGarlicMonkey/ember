#pragma once

namespace ember::inline graphics {
    enum class descriptor_type {
        uniform_buffer,
        storage_buffer,
        sampled_image,
        storage_image,
        sampler,
    };
}