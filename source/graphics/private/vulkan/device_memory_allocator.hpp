#pragma once

#include <list>
#include <vulkan/vulkan.h>
#include <ember/memory/memory.hpp>
#include <ember/containers/array.hpp>

namespace ember::graphics {
    /**
     * @brief Handles allocating memory from vulkan's VkDeviceMemory.
     * @details memory_allocator pools blocks of 256MB. It will sub-allocate
     * regions of those blocks as required, keeping memory contiguous and byte aligned.
     * If an allocation is larger than 256MB then a new VkDeviceMemory is created
     * of the required size.
     */
    class device_memory_allocator {
        //TYPES
    public:
        /**
         * @brief A portion of allocated memory inside a VkDeviceMemory allocation.
         */
        struct chunk {
            VkDeviceSize offset{ 0 };
            VkDeviceSize bytes{ 0 };

            VkDeviceMemory memory{ VK_NULL_HANDLE };

            bool free{ true };

            friend bool operator==(chunk const &lhs, chunk const &rhs);
        };

    private:
        /**
         * @brief A single VkDeviceMemory allocation.
         */
        class block {
            //VARIABLES
        private:
            VkDevice device{ VK_NULL_HANDLE };

            VkDeviceMemory memory{ VK_NULL_HANDLE };
            VkDeviceSize bytes{ 0 };
            std::uint32_t memory_type_index{ 0 };

            //TODO: custom linked list
            std::list<chunk> chunks{};

            //FUNCTIONS
        public:
            block() = delete;
            inline block(VkDevice device, VkDeviceMemory memory, VkDeviceSize bytes, std::uint32_t memory_type_index);

            block(block const &other) = delete;
            inline block(block &&other) noexcept;

            block &operator=(block const &other) = delete;
            inline block &operator=(block &&other) noexcept;

            inline ~block();

            chunk const *alloc(VkDeviceSize const bytes, VkDeviceSize const alignment);
            bool free(chunk const *&chunk_ptr);

            inline VkDeviceSize get_bytes() const;

            inline std::uint32_t get_memory_type_index() const;
        };

        //VARIABLES
    private:
        static VkDeviceSize constexpr default_block_size{ EMBER_MB(256) };

        VkDevice logical_device{ VK_NULL_HANDLE };
        VkPhysicalDevice physical_device{ VK_NULL_HANDLE };

        containers::array<block> memory_blocks{};

        //FUNCTIONS
    public:
        device_memory_allocator() = delete;
        inline device_memory_allocator(VkDevice logical_device, VkPhysicalDevice physical_device);

        device_memory_allocator(device_memory_allocator const &other) = delete;
        inline device_memory_allocator(device_memory_allocator &&other) noexcept;

        device_memory_allocator &operator=(device_memory_allocator const &other) = delete;
        inline device_memory_allocator &operator=(device_memory_allocator &&other) noexcept;

        inline ~device_memory_allocator();

        chunk const *alloc(VkMemoryRequirements const &memory_requirements, VkMemoryPropertyFlags const properties);
        void free(chunk const *&chunk);
    };
}

#include "device_memory_allocator.inl"