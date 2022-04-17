#pragma once

namespace ember::graphics {
    /**
     * @brief 
     */
    class shader {
        //TYPES
    public:
        enum class stage {
            vertex,
            pixel,
            compute,
        };

        //FUNCTIONS
    public:
        virtual ~shader() = default;
    };
}
