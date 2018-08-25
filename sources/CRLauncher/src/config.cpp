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

void show_error_box(const std::wstring& err_msg)
{
    MessageBoxW(NULL, err_msg.c_str(), L"CRLauncher Error", MB_ICONERROR);
}

bool load_panda3d_dlls(const boost::filesystem::path& thirdparty_path)
{
    const auto panda3d_path = thirdparty_path / "panda3d";
    if (!boost::filesystem::exists(panda3d_path))
    {
        show_error_box(L"Cannot find panda3d directory in thirdparty.");
        return false;
    }

    const auto bin_path = panda3d_path / "bin";
    if (!boost::filesystem::exists(bin_path))
    {
        show_error_box(L"Cannot find thirdparty/render_pipeline/bin directory.");
        return false;
    }

    try
    {
#if !defined(_DEBUG)
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(bin_path / "libp3dtool.dll", boost::dll::load_mode::append_decorations));
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(bin_path / "libp3dtoolconfig.dll", boost::dll::load_mode::append_decorations));
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(bin_path / "libpandaexpress.dll", boost::dll::load_mode::append_decorations));
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(bin_path / "libpanda.dll", boost::dll::load_mode::append_decorations));
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(bin_path / "libp3framework.dll", boost::dll::load_mode::append_decorations));
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(bin_path / "libp3direct.dll", boost::dll::load_mode::append_decorations));
#endif
        return true;
    }
    catch (const boost::system::system_error&)
    {
        show_error_box(L"Failed to load Panda3D DLLs.");
        return false;
    }
}

bool load_render_pipeline_dlls(const boost::filesystem::path& thirdparty_path)
{
    const auto render_pipeline_path = thirdparty_path / "render_pipeline";
    if (!boost::filesystem::exists(render_pipeline_path))
    {
        show_error_box(L"Cannot find render_pipeline directory in thirdparty.");
        return false;
    }

    const auto render_pipeline_bin_path = render_pipeline_path / "bin";
    if (!boost::filesystem::exists(render_pipeline_bin_path))
    {
        show_error_box(L"Cannot find thirdparty/render_pipeline/bin directory.");
        return false;
    }

#if defined(_DEBUG)
    const auto render_pipeline_dll_path = render_pipeline_bin_path / (std::string("render_pipeline-debug"));
#else
    const auto render_pipeline_dll_path = render_pipeline_bin_path / (std::string("render_pipeline"));
#endif

    try
    {
        dlls_.push_back(std::make_unique<boost::dll::shared_library>(
            render_pipeline_dll_path,
            boost::dll::load_mode::append_decorations));
        return true;
    }
    catch (const boost::system::system_error&)
    {
        show_error_box(std::wstring(L"Failed to load render_pipeline DLL: ") + render_pipeline_dll_path.native());
        return false;
    }
}

bool setup_path_env(const boost::filesystem::path& bin_path)
{
#if defined(_WIN32)
    const size_t BUFSIZE = 32767;
    LPWSTR pszOldVal = (LPWSTR)malloc(BUFSIZE * sizeof(WCHAR));
    if (!pszOldVal)
    {
        show_error_box(L"Out of memory");
        return false;
    }

    bool exit_flag = true;
    auto dwRet = GetEnvironmentVariableW(L"PATH", pszOldVal, BUFSIZE);
    if (0 == dwRet)
    {
        if (ERROR_ENVVAR_NOT_FOUND == GetLastError())
            exit_flag = false;
    }
    else if (BUFSIZE < dwRet)
    {
        pszOldVal = (LPWSTR)realloc(pszOldVal, dwRet * sizeof(WCHAR));
        if (!pszOldVal)
        {
            show_error_box(L"Out of memory");
            return false;
        }

        auto dwRet = GetEnvironmentVariableW(L"PATH", pszOldVal, BUFSIZE);
        if (!dwRet)
        {
            show_error_box(L"GetEnvironmentVariable failed (" + std::to_wstring(GetLastError()) + L")");
            free(pszOldVal);
            return false;
        }
        else
        {
            exit_flag = true;
        }
    }
    else
    {
        exit_flag = true;
    }

    auto new_env = std::wstring(pszOldVal) + L";" + bin_path.c_str() + L";";
    free(pszOldVal);

    if (!SetEnvironmentVariableW(L"PATH", new_env.c_str()))
    {
        show_error_box(L"SetEnvironmentVariable failed: " + std::to_wstring(GetLastError()));
        return false;
    }
#endif

    return true;
}

bool load_dlls()
{
#if defined(_WIN32)
	const auto launcher_dir_path = boost::dll::this_line_location().parent_path();
	if (!launcher_dir_path.has_parent_path())
	{
        show_error_box(L"Cannot find CRSF engine directory.");
		return false;
	}

	const auto engine_path = launcher_dir_path.parent_path();
	const auto engine_bin_path = engine_path / "bin";
	const auto thirdparty_path = engine_path / "thirdparty";
	if (!(boost::filesystem::exists(engine_bin_path) && boost::filesystem::exists(thirdparty_path)))
	{
        show_error_box(L"Cannot find CRSF engine and thirdparty directory.");
		return false;
	}

    if (!setup_path_env(engine_bin_path))
        return false;

    if (!load_panda3d_dlls(thirdparty_path))
        return false;

    if (!load_render_pipeline_dlls(thirdparty_path))
        return false;

#if defined(CRLAUNCHER_BUILD_CFG_POSTFIX)
	const auto crseedlib_dll_path = engine_bin_path / (std::string("CRSeedLib") + CRLAUNCHER_BUILD_CFG_POSTFIX + boost::dll::shared_library::suffix().generic_string());
#else
	const auto crseedlib_dll_path = engine_bin_path / (std::string("CRSeedLib") + boost::dll::shared_library::suffix().generic_string());
#endif

	if (!boost::filesystem::exists(crseedlib_dll_path))
	{
        show_error_box(L"Cannot find CRSeedLib DLL.");
		return false;
	}
#endif

	return true;
}
