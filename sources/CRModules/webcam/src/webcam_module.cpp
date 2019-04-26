/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#include "webcam_module.h"

#include <spdlog/logger.h>

#include <crsf/RealworldInterface/TInterfaceManager.h>
#include <crsf/CRModel/TPhysicalCamera.h>
#include <crsf/RenderingEngine/TGraphicRenderEngine.h>

#include "webcam_interface.h"

CRSEEDLIB_MODULE_CREATOR(WebcamModule);

spdlog::logger* module_logger_;

WebcamModule::WebcamModule(void) : TDynamicModuleInterface(CRMODULE_ID_STRING)
{
    module_logger_ = m_logger.get();
}

WebcamModule::~WebcamModule()
{
    module_logger_ = nullptr;
}

void WebcamModule::OnLoad(void)
{
    crsf::TInterfaceManager *pInterfaceManager = crsf::TInterfaceManager::GetInstance();

    const std::string& name = m_property.get("input_interface", std::string("webcam"));

    m_logger->debug("Webcam Input Interface name: {}", name);

    int cam_index = m_property.get<int>("camindex", 0);

    webcam_interface_ = std::make_unique<WebcamInterface>(cam_index);
    pInterfaceManager->AddInputInterface(name, webcam_interface_.get(), crsf::TCRINTERFACE_CAM);
}

void WebcamModule::OnStart(void)
{
}

void WebcamModule::OnExit(void)
{
    webcam_interface_.reset();
}
