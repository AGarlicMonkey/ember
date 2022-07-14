#pragma once

#include "ember/ecs/types.hpp"

namespace ember::inline ecs::internal {
    class component_helpers {
    public:
        virtual void move(std::byte *source, std::byte *destination) const = 0;
        virtual void destruct(std::byte *memory) const                     = 0;

        virtual std::size_t get_size() const  = 0;
        virtual component_id_t get_id() const = 0;
    };

    template<typename component_t>
    class component_helpers_impl : public component_helpers {
    public:
        void move(std::byte *source, std::byte *destination) const final;
        void destruct(std::byte *memory) const final;

        std::size_t get_size() const final;
        component_id_t get_id() const final;
    };
}

#include "component_helper.inl"