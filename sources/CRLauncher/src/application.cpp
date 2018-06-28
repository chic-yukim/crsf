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

#include <crsf/System/TSystemConfiguration.h>
#include <crsf/RealworldInterface/TInterfaceManager.h>
#include <crsf/RenderingEngine/TGraphicRenderEngine.h>
#include <crsf/CREngine/TDynamicModuleManager.h>

#include "config.h"

AsyncTask::DoneStatus load_application(rppanda::FunctionalTask*);
AsyncTask::DoneStatus start_application(rppanda::FunctionalTask*);

AsyncTask::DoneStatus initiailze_application(rppanda::FunctionalTask*)
{
	global_logger_->trace("Initializing main application.");

	crsf::TInterfaceManager::GetInstance()->Init();

	// load TDynamicModuleManager
	crsf::TDynamicModuleManager::GetInstance()->LoadModules();

	rppanda::TaskManager::get_global_instance()->add(load_application, "CRLauncher::load_application");

	return AsyncTask::DS_done;
}

AsyncTask::DoneStatus load_application(rppanda::FunctionalTask*)
{
	global_logger_->trace("Loading main application.");

	crsf::TDynamicModuleManager::GetInstance()->OnLoad();

	crsf::TInterfaceManager::GetInstance()->ConnectInputOutputInterface();

	rppanda::TaskManager::get_global_instance()->add(start_application, "CRLauncher::start_application");

	return AsyncTask::DS_done;
}

AsyncTask::DoneStatus start_application(rppanda::FunctionalTask*)
{
	global_logger_->trace("Starting main application.");

	crsf::TDynamicModuleManager::GetInstance()->OnStart();

	return AsyncTask::DS_done;
}

void exit_application(void)
{
	global_logger_->trace("Exiting main application.");

	crsf::TInterfaceManager::GetInstance()->Exit();
	crsf::TDynamicModuleManager::GetInstance()->OnExit();
	crsf::TGraphicRenderEngine::GetInstance()->Exit();
}
