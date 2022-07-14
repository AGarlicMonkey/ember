#pragma once

#include "ember/graphics/image.hpp"

#include <ember/containers/array.hpp>
#include <optional>

namespace ember::inline graphics {
    enum class load_operation {
        load,
        clear,
        dont_care,
    };

    enum class store_operation {
        store,
        dont_care,
    };

    /**
     * @brief Describes what happens to an attachment when it is loaded and stored.
     */
    struct attachment_descriptor {
        image::format format{ image::format::unknown }; /**< The expected format of the image. */

        load_operation load_op{ load_operation::dont_care };    /**< What to do with the attchment when it is loaded. */
        store_operation store_op{ store_operation::dont_care }; /**< How to store the attachment. */

        image::layout initial_layout{ image::layout::undefined }; /**< What layout the render pass will expect the attachment to be in. */
        image::layout used_layout{ image::layout::undefined };    /**< What layout the attachment will transition to when used during the render pass. */
        image::layout final_layout{ image::layout::undefined };   /**< What layout the attachment will be transitioned to at the end of the render pass.*/
    };
}

namespace ember::inline graphics {
    /**
     * @brief A render_pass describes a set of attachments that graphics_pipeline_object can render to.
     * @details The actual attchments themselves are stored in a framebuffer object. This class is used
     * to describe the type of attachments.
     */
    class render_pass {
        //TYPES
    public:
        struct descriptor {
            array<attachment_descriptor> colour_attachments{};       /**< Array of attachment descriptors for each colour output attachment. */
            std::optional<attachment_descriptor> depth_attachment{}; /**< Optional descriptor for the depth attachment. */
        };

        //FUNCTIONS
    public:
        virtual ~render_pass() = default;

        virtual descriptor const &get_descriptor() const = 0;
    };
}