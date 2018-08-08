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

#include <crsf/CoexistenceInterface/TDynamicStageMemory.h>
#include <crsf/RemoteWorldInterface/TNetworkManager.h>
#include <crsf/CREngine/TPhysicsManager.h>

CRSEEDLIB_MODULE_CREATOR(CRProfilerModule);

CRProfilerModule::CRProfilerModule(void): TDynamicModuleInterface(CRMODULE_ID_STRING)
{
}

void CRProfilerModule::OnLoad(void)
{
}

void CRProfilerModule::OnStart(void)
{
    dsm_ = crsf::TDynamicStageMemory::GetInstance();
    nm_ = crsf::TNetworkManager::GetInstance();
    pm_ = crsf::TPhysicsManager::GetInstance();

    rppanda::Messenger::get_global_instance()->send(
        "imgui-setup-context",
        EventParameter(new rppanda::FunctionalTask([this](rppanda::FunctionalTask* task) {
            ImGui::SetCurrentContext(std::static_pointer_cast<ImGuiContext>(task->get_user_data()).get());
            accept("imgui-new-frame", [this](const Event*) { on_imgui_new_frame(); });
            return AsyncTask::DS_done;
        }, "setup-imgui"))
    );
}

void CRProfilerModule::OnExit(void)
{
    remove_all_tasks();
    ignore_all();
}

void CRProfilerModule::on_imgui_new_frame()
{
    static bool window = true;
    enum class ToolsType
    {
        none = 0,
        dynamic_stage_memory,
        network_manager,
        physics_manager,
    };
    static ToolsType tools_type = ToolsType::none;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

    ImGui::Begin("CRProfiler", &window, window_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Dynamic Stage Memory"))
                tools_type = ToolsType::dynamic_stage_memory;

            if (ImGui::MenuItem("Network Manager"))
                tools_type = ToolsType::network_manager;

            if (ImGui::MenuItem("Physics Manager"))
                tools_type = ToolsType::physics_manager;

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (tools_type == ToolsType::dynamic_stage_memory)
    {
        ImGui::LabelText("System Index", "%d", dsm_->GetSystemIndex());

        if (ImGui::CollapsingHeader("Image Memory Object"))
            on_imgui_image_mo();

        if (ImGui::CollapsingHeader("Avatar Memory Object"))
            on_imgui_avatar_mo();

        if (ImGui::CollapsingHeader("Point Memory Object"))
            on_imgui_point_mo();

        if (ImGui::CollapsingHeader("Command Memory Object"))
            on_imgui_command_mo();

        if (ImGui::CollapsingHeader("Control Memory Object"))
            on_imgui_control_mo();
    }
    else if (tools_type == ToolsType::network_manager)
    {
        on_imgui_network_manager();
    }
    else if (tools_type == ToolsType::physics_manager)
    {
        on_imgui_physics_manager();
    }

    ImGui::End();
}
