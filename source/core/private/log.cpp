#include "ember/core/log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

static ember::logger *instance{ nullptr };

namespace ember {
    logger::logger() {
        //Always add a console sink by default. Any other sinks should be added manually.
        auto console_sink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
        console_sink->set_pattern("%^[%T] %v%$");

        std::vector<spdlog::sink_ptr> sinks{ console_sink };
        internal_logger = std::make_unique<spdlog::logger>("ember_logger", sinks.begin(), sinks.end());
        internal_logger->set_level(spdlog::level::trace);
    }

    logger::~logger() {
        flush();
    }

    logger &logger::get() {
        if(instance == nullptr) {
            instance = new logger{};
        }

        return *instance;
    }

    void logger::flush() {
        internal_logger->flush();
    }

    void logger::add_sink(std::shared_ptr<spdlog::sinks::sink> sink) {
        internal_logger->sinks().push_back(std::move(sink));
    }

    void logger::do_log(log_level level, std::string_view msg) {
        switch(level) {
            case log_level::trace:
                internal_logger->trace(msg);
                break;
            case log_level::debug:
                internal_logger->debug(msg);
                break;
            case log_level::info:
                internal_logger->info(msg);
                break;
            case log_level::warn:
                internal_logger->warn(msg);
                break;
            case log_level::error:
                internal_logger->error(msg);
                break;
            case log_level::critical:
                internal_logger->critical(msg);
                break;
        }
    }
}