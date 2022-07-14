#pragma once

#include <tuple>
#include <cstddef>

namespace ember::inline ecs::internal {
    template<typename function_t>
    struct function_traits;

    template<typename return_t, typename... parameters_t>
    struct function_traits<return_t(parameters_t...)> {
        using return_type                   = return_t;
        using parameter_types_tuple         = std::tuple<parameters_t...>;
        using decayed_parameter_types_tuple = std::tuple<std::decay_t<parameters_t>...>;

        static inline std::size_t constexpr arity{ sizeof...(parameters_t) };
    };

    //Free function specialisation
    template<typename return_t, typename... parameters_t>
    struct function_traits<return_t (*)(parameters_t...)> : public function_traits<return_t(parameters_t...)> {};

    //Member function specialisation
    template<typename return_t, typename class_t, typename... parameters_t>
    struct function_traits<return_t (class_t::*)(parameters_t...)> : public function_traits<return_t(parameters_t...)> {};

    //Const member function specialisation
    template<typename return_t, typename class_t, typename... parameters_t>
    struct function_traits<return_t (class_t::*)(parameters_t...) const> : public function_traits<return_t(parameters_t...)> {};

    //Callable function specialisation
    template<typename callable_t>
    struct function_traits : public function_traits<decltype(&callable_t::operator())> {};
}