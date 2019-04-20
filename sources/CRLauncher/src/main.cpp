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

#include <render_pipeline/rppanda/task/task_manager.hpp>

#include <boost/program_options.hpp>

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>
#include <crsf/System/TLogger.h>
#include <crsf/System/TSystemConfiguration.h>

#include "config.h"

void setup_program_options(boost::program_options::options_description& desc);
AsyncTask::DoneStatus initiailze_application(rppanda::FunctionalTask*);
void exit_application(void);

int main(int argc, char* argv[])
{
    boost::program_options::options_description option_desc("Options");
    setup_program_options(option_desc);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, option_desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << option_desc << std::endl;
        return 0;
    }

    if (vm.count("config-file"))
    {
        crsf::TSystemConfiguration::GetInstance()->ChangeSystemPropertyFilePath(vm["config-file"].as<std::string>());
    }

    crsf::InitializeLoggerSink();
    InitLogging();

    crsf::TGraphicRenderEngine* rendering_engine = crsf::TGraphicRenderEngine::GetInstance();
    rendering_engine->SetRenderingScaleFactor(5.0f);

    rppanda::TaskManager::get_global_instance()->add(initiailze_application, "CRLauncher::initiailze_application");

    // initialize and loop rendering engine.
    rendering_engine->Start();

    exit_application();

    rendering_engine->Exit();

    // clear rendering engine.
    rendering_engine->Clear();

    crsf::FinalizeLoggerSink();

    return 0;
}
