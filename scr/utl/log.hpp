#ifndef LOG_HPP_GUARD
#define LOG_HPP_GUARD

#include <memory>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// Konsole + logs/everything.log (alles) + logs/error.log (nur Fehler)
inline auto init_logging(const std::string &dir = "logs") -> void {
	auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto everything = std::make_shared<spdlog::sinks::basic_file_sink_mt>(dir + "/everything.log", false);
	auto errors = std::make_shared<spdlog::sinks::basic_file_sink_mt>(dir + "/error.log", true);
	errors->set_level(spdlog::level::err);

	auto logger = std::make_shared<spdlog::logger>("gb", spdlog::sinks_init_list{console, everything, errors});
	logger->set_level(spdlog::level::trace);
	logger->flush_on(spdlog::level::err);
	spdlog::set_default_logger(logger);
}

#endif // !LOG_HPP_GUARD
