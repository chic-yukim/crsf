/**
 * MIT License
 *
 * Copyright (c) 2016-2018 Center of Human-centered Interaction for Coexistence
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "config.h"

#include <boost/filesystem.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>

#include <crsf/System/TSystemConfiguration.h>

#include <iostream>

#if defined(SPDLOG_VER_MAJOR)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#endif

std::shared_ptr<spdlog::logger> global_logger_;
static std::vector<std::unique_ptr<boost::dll::shared_library>> dlls_;

void InitLogging(void)
{
	const auto& system_config = crsf::TSystemConfiguration::GetInstance()->GetSystemProperty();

	const std::string logging_level = system_config.get("system.logging.level", std::string("info"));

	std::vector<spdlog::sink_ptr> sinks;
#ifdef _WIN32
	sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
	sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_sink>(spdlog::sinks::stdout_sink_mt::instance()));
#endif

#if defined(SPDLOG_VER_MAJOR)
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("crlauncher.log", true));
#else
	sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("crlauncher.log", true));
#endif

	global_logger_ = std::make_shared<spdlog::logger>("CRLauncher", std::begin(sinks), std::end(sinks));
#if defined(SPDLOG_VER_MAJOR)
	global_logger_->set_pattern("%^[%H:%M:%S.%e] [%t] [%l] [%n] %v%$");
#else
    global_logger_->set_pattern("[%H:%M:%S.%e] [%t] [%l] [%n] %v");
#endif
	global_logger_->flush_on(spdlog::level::err);

	if (logging_level == "trace")
	{
		global_logger_->set_level(spdlog::level::trace);
	}
	else if (logging_level == "debug")
	{
		global_logger_->set_level(spdlog::level::debug);
	}
	else if (logging_level == "info")
	{
		global_logger_->set_level(spdlog::level::info);
	}
	else if (logging_level == "warn")
	{
		global_logger_->set_level(spdlog::level::warn);
	}
	else if (logging_level == "error")
	{
		global_logger_->set_level(spdlog::level::err);
	}
	else
	{
		global_logger_->error("Invalid logging level: {}", logging_level);
	}
}
