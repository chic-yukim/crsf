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

#pragma once

#include <luse.h>

#include <render_pipeline/rppanda/showbase/direct_object.hpp>

#include <crsf/CRAPI/TDynamicModuleInterface.h>

namespace crsf {
class TDynamicStageMemory;
class TNetworkManager;
class TPhysicsManager;
}

class CRProfilerModule: public crsf::TDynamicModuleInterface, public rppanda::DirectObject
{
public:
    CRProfilerModule(void);

    void OnLoad(void) override;
    void OnStart(void) override;
    void OnExit(void) override;

private:
    void on_imgui_new_frame();
    void on_imgui_main();
    void on_imgui_image_mo();
    void on_imgui_avatar_mo();
    void on_imgui_point_mo();
    void on_imgui_command_mo();
    void on_imgui_control_mo();
    void on_imgui_sound_mo();

    void on_imgui_network_manager();
    void on_imgui_physics_manager();

    crsf::TDynamicStageMemory* dsm_ = nullptr;
    crsf::TNetworkManager* nm_ = nullptr;
    crsf::TPhysicsManager* pm_ = nullptr;
};
