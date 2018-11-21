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
#include <crsf/CoexistenceInterface/TPointMemoryObject.h>
#include <crsf/CoexistenceInterface/TAvatarMemoryObject.h>
#include <crsf/CoexistenceInterface/TCommandMemoryObject.h>
#include <crsf/CoexistenceInterface/TControlMemoryObject.h>
#include <crsf/CoexistenceInterface/TSoundMemoryObject.h>
#include <crsf/RenderingEngine/GraphicRenderEngine/TTexture.h>

void CRProfilerModule::on_imgui_image_mo()
{
    static std::string imo_name;
    static crsf::TImageMemoryObject* current_imo = nullptr;
    if (current_imo)
        imo_name = current_imo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name###image_mo", imo_name.c_str()))
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

    if (!current_imo)
        return;

    ImGui::LabelText("System Index", "%d", current_imo->GetSystemIndex());
    ImGui::LabelText("Listener Count", "%d", current_imo->GetListenerCount());
    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_imo->GetImageMemorySize()).c_str());

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
    static std::string mo_name;
    static crsf::TAvatarMemoryObject* current_mo = nullptr;
    if (current_mo)
        mo_name = current_mo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name###avatar_mo", mo_name.c_str()))
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

    ImGui::LabelText("System Index", "%d", current_mo->GetSystemIndex());
    ImGui::LabelText("Listener Count", "%d", current_mo->GetListenerCount());
    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_mo->GetBinaryAvatarMemorySize()).c_str());
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

            static bool show_matrix = false;
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Selectable(fmt::format("Show {}", show_matrix ? "Matrix" : "Position/Rotation/Scale").c_str()))
                    show_matrix = !show_matrix;
                ImGui::EndPopup();
            }

            index = (std::max)(0, (std::min)(index, int(poses.size() - 1)));

            static const ImGuiInputTextFlags flag = ImGuiInputTextFlags_ReadOnly;
            if (show_matrix)
            {
                LMatrix4f mat = poses[index].GetMatrix();

                ImGui::InputFloat4("Row 0", &mat(0, 0), -1, flag);
                ImGui::InputFloat4("Row 1", &mat(1, 0), -1, flag);
                ImGui::InputFloat4("Row 2", &mat(2, 0), -1, flag);
                ImGui::InputFloat4("Row 3", &mat(3, 0), -1, flag);
            }
            else
            {
                LVecBase3f scale;
                LVecBase3f pos;
                LVecBase3f hpr;
                LVecBase3f shear;
                poses[index].DecomposeMatrix(scale, shear, hpr, pos);

                ImGui::InputFloat3("Position", &pos[0], -1, flag);
                ImGui::InputFloat3("HPR", &hpr[0], -1, flag);
                ImGui::InputFloat3("Scale", &scale[0], -1, flag);
                ImGui::InputFloat3("Shear", &shear[0], -1, flag);
            }
        }
        ImGui::TreePop();
    }
}

void CRProfilerModule::on_imgui_point_mo()
{
    static std::string mo_name;
    static crsf::TPointMemoryObject* current_mo = nullptr;
    if (current_mo)
        mo_name = current_mo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name###point_mo", mo_name.c_str()))
    {
        for (size_t k = 0, k_end = dsm_->GetNumPointMemoryObject(); k < k_end; ++k)
        {
            auto mo = dsm_->GetPointMemoryObjectByIndex(k);

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

    ImGui::LabelText("System Index", "%d", current_mo->GetSystemIndex());
    ImGui::LabelText("Listener Count", "%d", current_mo->GetListenerCount());
    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_mo->GetPointMemorySize()).c_str());

    const auto& pm = current_mo->GetPointMemory();
    if (pm.empty())
        return;

    static int index = 0;
    ImGui::InputInt("Pose Index", &index);

    index = (std::max)(0, (std::min)(index, int(pm.size() - 1)));

    LVecBase3f scale;
    LVecBase3f pos;
    LVecBase3f hpr;
    LVecBase3f shear;
    pm[index].m_Pose.DecomposeMatrix(scale, shear, hpr, pos);

    const ImGuiInputTextFlags flag = ImGuiInputTextFlags_ReadOnly;
    ImGui::LabelText("Index", "%d", pm[index].m_nIndex);
    ImGui::InputFloat3("Position", &pos[0], -1, flag);
    ImGui::InputFloat3("HPR", &hpr[0], -1, flag);
    ImGui::InputFloat3("Scale", &scale[0], -1, flag);
    ImGui::InputFloat3("Shear", &shear[0], -1, flag);
}

void CRProfilerModule::on_imgui_command_mo()
{
    static std::string mo_name;
    static crsf::TCommandMemoryObject* current_mo = nullptr;
    if (current_mo)
        mo_name = current_mo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name###command_mo", mo_name.c_str()))
    {
        for (size_t k = 0, k_end = dsm_->GetNumCommandMemoryObject(); k < k_end; ++k)
        {
            auto mo = dsm_->GetCommandMemoryObjectByIndex(k);

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

    ImGui::LabelText("Listener Count", "%d", current_mo->GetListenerCount());
    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_mo->GetCommandMemorySize()).c_str());

    for (size_t k = 0; k < COMMAND_MAXSIZE; ++k)
        ImGui::Text("%d : %d", k, current_mo->GetCommandMemory()[k]);
}

void CRProfilerModule::on_imgui_control_mo()
{
    static std::string mo_name;
    static crsf::TControlMemoryObject* current_mo = nullptr;
    if (current_mo)
        mo_name = current_mo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name###control_mo", mo_name.c_str()))
    {
        for (size_t k = 0, k_end = dsm_->GetNumControlMemoryObject(); k < k_end; ++k)
        {
            auto mo = dsm_->GetControlMemoryObjectByIndex(k);

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

    ImGui::LabelText("System Index", "%d", current_mo->GetSystemIndex());
    ImGui::LabelText("Listener Count", "%d", current_mo->GetListenerCount());
    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_mo->GetControlMemorySize()).c_str());

    for (size_t k = 0; k < CONTROL_MAXSIZE; ++k)
        ImGui::Text("%d : %d", k, current_mo->GetControlMemory()[k]);
}

void CRProfilerModule::on_imgui_sound_mo()
{
    static std::string mo_name;
    static crsf::TSoundMemoryObject* current_mo = nullptr;
    if (current_mo)
        mo_name = current_mo->GetProperty().m_strName;

    bool is_changed = false;
    if (ImGui::BeginCombo("Name###sound_mo", mo_name.c_str()))
    {
        for (size_t k = 0, k_end = dsm_->GetNumSoundMemoryObject(); k < k_end; ++k)
        {
            auto mo = dsm_->GetSoundMemoryObjectByIndex(k);

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

    ImGui::LabelText("System Index", "%d", current_mo->GetSystemIndex());
    ImGui::LabelText("Listener Count", "%d", current_mo->GetListenerCount());
    ImGui::LabelText("Memory Size (bytes)", std::to_string(current_mo->GetSoundMemorySize()).c_str());

    const auto& props = current_mo->GetSoundProp();
    ImGui::LabelText("Channel (In)", "%d", props.m_nInChannel);
    ImGui::LabelText("Channel (Out)", "%d", props.m_nOutChannel);
    ImGui::LabelText("Buffer Size", "%d", props.m_nBufferSize);
    ImGui::LabelText("Sample Size", "%d", props.m_nSampleSize);
}
