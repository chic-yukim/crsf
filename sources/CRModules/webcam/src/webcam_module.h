/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#pragma once

#include <crsf/CRAPI/TDynamicModuleInterface.h>

class WebcamInterface;

class WebcamModule: public crsf::TDynamicModuleInterface
{
public:
    WebcamModule();
    ~WebcamModule() override;

    void OnLoad() override;
    void OnStart() override;
    void OnExit() override;

private:
    std::unique_ptr<WebcamInterface> webcam_interface_;
};
