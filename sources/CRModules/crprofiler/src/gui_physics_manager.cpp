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

#include "crmodules/crprofiler/module.hpp"

#include <imgui.h>

#include <fmt/format.h>

#include <crsf/CREngine/TPhysicsManager.h>

void CRProfilerModule::on_imgui_physics_manager()
{
    if (!pm_->IsInit())
    {
        ImGui::Text("Physics Manager is not initialized.");
        return;
    }

    ImGui::LabelText("Engine Type", "%d", pm_->GetPhysicsEngineType());

    if (!pm_->IsStarted())
        return;

    int fps = pm_->GetFPS();
    ImGui::InputInt("FPS", &fps, 1, 100, ImGuiInputTextFlags_ReadOnly);

    LVecBase3f gravity = pm_->GetGravity();
    if (ImGui::InputFloat3("Gravity", &gravity[0]))
        pm_->SetGravity(gravity);
}
