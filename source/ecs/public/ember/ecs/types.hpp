#pragma once

#include <cinttypes>
#include <ember/containers/array.hpp>

namespace ember::inline ecs {
    using id_t           = std::uint32_t;
    using entity         = id_t;
    using component_id_t = id_t;
    using archetype_id_t = array<component_id_t>;

    inline entity constexpr null_entity{ 0 };

    class id_generator {
    private:
        inline static id_t count{ 0 };

    public:
        template<typename U>
        static id_t get() {
            static id_t const id_counter{ count++ };
            return id_counter;
        }
    };
}