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
#include <crsf/CoexistenceInterface/TImageMemoryObject.h>
#include <crsf/CoexistenceInterface/TAvatarMemoryObject.h>
#include <crsf/RenderingEngine/GraphicRenderEngine/TTexture.h>

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

    ImGui::Begin("CRProfiler", &window);

    if (ImGui::CollapsingHeader("Image Memory Object"))
        on_imgui_image_mo();

    if (ImGui::CollapsingHeader("Avatar Memory Object"))
        on_imgui_avatar_mo();

    ImGui::End();
}

void CRProfilerModule::on_imgui_image_mo()
{
    static ImGuiComboFlags comobo_flags = 0;
    static std::string imo_name;
    static crsf::TImageMemoryObject* current_imo = nullptr;
    if (current_imo)
        imo_name = current_imo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name", imo_name.c_str(), comobo_flags))
    {
        for (size_t k = 0, k_end = dsm_->GetNumImageMemoryObject(); k < k_end; ++k)
        {
            auto imo = dsm_->GetImageMemoryObjectByIndex(k);

            bool is_selected = (current_imo == imo);
            auto id = fmt::format("{}###{}", imo->GetProperty().m_strName, (void*)imo);
            if (ImGui::Selectable(id.c_str(), is_selected))
            {
                current_imo = imo;
                is_changed = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
    }

    static std::unique_ptr<crsf::TTexture> tex = nullptr;

    if (is_changed)
    {
        tex = std::make_unique<crsf::TTexture>("CRProfilerIMOTexture");
        tex->SetupFromImageMemoryObject(current_imo, false);
    }

    ImGui::Text("Preview");
    ImGui::Image(
        tex ? tex->GetPandaTexture() : nullptr,
        ImVec2(256, 256),
        ImVec2(0, 0),
        ImVec2(1, 1),
        ImColor(255, 255, 255, 255),
        ImColor(255, 255, 255, 128));
}

void CRProfilerModule::on_imgui_avatar_mo()
{
    static ImGuiComboFlags comobo_flags = 0;
    static std::string mo_name;
    static crsf::TAvatarMemoryObject* current_mo = nullptr;
    if (current_mo)
        mo_name = current_mo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name", mo_name.c_str(), comobo_flags))
    {
        for (size_t k = 0, k_end = dsm_->GetNumAvatarMemoryObject(); k < k_end; ++k)
        {
            auto mo = dsm_->GetAvatarMemoryObjectByIndex(k);

            bool is_selected = (current_mo == mo);
            auto id = fmt::format("{}###{}", mo->GetProperty().m_strName, (void*)mo);
            if (ImGui::Selectable(id.c_str(), is_selected))
            {
                current_mo = mo;
                is_changed = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
    }

    if (!current_mo)
        return;

    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_mo->GetAvatarMemorySize()).c_str());
    ImGui::LabelText("Joint Count", std::to_string(current_mo->GetAvatarProp().GetJointNumber()).c_str());

    if (ImGui::TreeNode("Avatar Headers"))
    {
        ImGui::Columns(5, "header_columns");
        ImGui::Separator();
        ImGui::Text("Index"); ImGui::NextColumn();
        ImGui::Text("Update"); ImGui::NextColumn();
        ImGui::Text("Width"); ImGui::NextColumn();
        ImGui::Text("Normal"); ImGui::NextColumn();
        ImGui::Text("Scaling Ratio"); ImGui::NextColumn();
        ImGui::Separator();

        const auto& headers = current_mo->GetAvatarHeader();
        if (headers.size() > 0)
        {
            static int index = 0;
            ImGui::InputInt("##header_index", &index);   ImGui::NextColumn();

            index = (std::max)(0, (std::min)(index, int(headers.size()-1)));

            const auto& header = headers[index];
            ImGui::Text(header.m_bUpdate ? "T" : "F");  ImGui::NextColumn();
            ImGui::Text("%f", header.m_fWidth);  ImGui::NextColumn();
            ImGui::Text("%f %f %f", header.m_vec3Normal[0], header.m_vec3Normal[1], header.m_vec3Normal[2]); ImGui::NextColumn();
            ImGui::Text("%f %f %f", header.m_vec3ScalingRatio[0], header.m_vec3ScalingRatio[1], header.m_vec3ScalingRatio[2]); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Avatar Pose"))
    {
        const auto& poses = current_mo->GetAvatarMemory();
        if (poses.size() > 0)
        {
            static int index = 0;
            ImGui::InputInt("Pose Index", &index);

            index = (std::max)(0, (std::min)(index, int(poses.size() - 1)));

            LVecBase3f scale;
            LVecBase3f pos;
            LVecBase3f hpr;
            LVecBase3f shear;
            poses[index].DecomposeMatrix(scale, shear, hpr, pos);

            const ImGuiInputTextFlags flag = ImGuiInputTextFlags_ReadOnly;
            ImGui::InputFloat3("Position", &pos[0], -1, flag);
            ImGui::InputFloat3("HPR", &hpr[0], -1, flag);
            ImGui::InputFloat3("Scale", &scale[0], -1, flag);
            ImGui::InputFloat3("Shear", &shear[0], -1, flag);
        }
        ImGui::TreePop();
    }
}