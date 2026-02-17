#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <projgeom/logger.hpp>

namespace projgeom {

void log_with_spdlog(const std::string& message) {
    // Always create a fresh logger to ensure proper file handling
    std::shared_ptr<spdlog::logger> logger;
    try {
        // Try to drop the existing logger first
        spdlog::drop("file_logger");
    } catch (...) {
        // Ignore if logger doesn't exist
    }

    // Create a new logger
    logger = spdlog::basic_logger_mt("file_logger", "projgeom.log");
    if (logger) {
        logger->set_level(spdlog::level::info);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
        logger->flush_on(spdlog::level::info);
        logger->info("ProjGeom message: {}", message);
        logger->flush();
    }
}

} // namespace projgeom