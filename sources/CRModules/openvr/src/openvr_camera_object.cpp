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

#include "openvr_camera_object.h"

#include <camera.h>

#include <spdlog/spdlog.h>

#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rppanda/showbase/showbase.hpp>
#include <render_pipeline/rppanda/showbase/messenger.hpp>

#include <rpplugins/openvr/plugin.hpp>
#include <rpplugins/openvr/camera_interface.hpp>

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>
#include <crsf/RenderingEngine/GraphicRenderEngine/TTexture.h>

const char* OpenVRCameraObject::UPDATE_EVENT_NAME = "OpenVRCameraObject::Update";

OpenVRCameraObject::OpenVRCameraObject(OpenVRModule& module_instance) :
	crsf::TPhysicalCamera(NodePath(module_instance.GetOpenVRPlugin()->get_tracked_camera()->create_camera_node(0))), module_(module_instance)
{
	SetName(module_.m_property.get("camera_name", std::string("openvr_camera")));
	SetDeviceName(GetName());
	SetBGRType(false);

	camera_ = module_.GetOpenVRPlugin()->get_tracked_camera();

	uint32_t buffer_size;
	camera_->get_frame_size(m_width, m_height, buffer_size);
	framebuffer_.resize(buffer_size);

	m_texture = std::make_shared<crsf::TTexture>(GetDeviceName());
	auto tex = m_texture->GetPandaTexture();
	tex->setup_2d_texture(m_width, m_height, Texture::ComponentType::T_unsigned_byte, Texture::Format::F_srgb_alpha);
    tex->set_wrap_u(SamplerState::WrapMode::WM_clamp);
    tex->set_wrap_v(SamplerState::WrapMode::WM_clamp);
    tex->set_magfilter(SamplerState::FilterType::FT_linear);
    tex->set_minfilter(SamplerState::FilterType::FT_linear_mipmap_linear);

	PTA_uchar ram_image = tex->make_ram_image();
	memset(ram_image.p(), 0xFF, m_width * m_height * 4);

	module_.m_logger->debug("OpenVR camera opened.");
}

AsyncTask::DoneStatus OpenVRCameraObject::Update(rppanda::FunctionalTask* task)
{
	const auto elapsed_time = task->get_elapsed_time();

	if ((elapsed_time - last_task_time_) < (16.0 / 1000.0))
		return AsyncTask::DS_cont;

	last_task_time_ = elapsed_time;

	vr::CameraVideoStreamFrameHeader_t new_frame_header;

	camera_->get_frame_header(new_frame_header);

	// frame hasn't changed yet, nothing to do
	if (new_frame_header.nFrameSequence == frame_header_.nFrameSequence)
		return AsyncTask::DS_cont;

	// Frame has changed, do the more expensive frame buffer copy
	auto err = camera_->get_framebuffer(new_frame_header, framebuffer_);
	if (err != vr::VRTrackedCameraError_None)
		return AsyncTask::DS_cont;

	PTA_uchar ram_image = m_texture->GetPandaTexture()->modify_ram_image();
	std::copy(framebuffer_.begin(), framebuffer_.end(), ram_image.p());

	const auto& pose = new_frame_header.standingTrackedDevicePose;
	if (pose.bPoseIsValid)
	{
		static const LMatrix4f z_to_y = LMatrix4f::convert_mat(CS_zup_right, CS_yup_right);
		static const LMatrix4f y_to_z = LMatrix4f::convert_mat(CS_yup_right, CS_zup_right);

		m_nodePath.set_mat(z_to_y *
			rpplugins::OpenVRPlugin::convert_matrix(pose.mDeviceToAbsoluteTracking) *
			y_to_z);
	}

	frame_header_ = new_frame_header;

    rpcore::Globals::base->get_messenger()->send("OpenVRCameraObject::Update", false);

	return AsyncTask::DS_cont;
}

bool OpenVRCameraObject::StartStream()
{
	if (!camera_->acquire_video_streaming_service())
		return false;

	update_task_ = rpcore::Globals::base->add_task(
		std::bind(&OpenVRCameraObject::Update, this, std::placeholders::_1),
		"OpenVRCameraObject::Update", -50);

	return true;
}

void OpenVRCameraObject::StopStream()
{
	rpcore::Globals::base->remove_task(update_task_->get_name());
	camera_->release_video_streaming_service();
}
