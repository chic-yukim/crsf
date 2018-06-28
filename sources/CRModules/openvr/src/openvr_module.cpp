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

#include "openvr_module.h"

#include <render_pipeline/rpcore/pluginbase/manager.hpp>
#include <render_pipeline/rpcore/render_pipeline.hpp>
#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rppanda/showbase/showbase.hpp>

#include <openvr_plugin.hpp>
#include <openvr_camera_interface.hpp>

#include <spdlog/spdlog.h>

#include <crsf/RenderingEngine/TGraphicRenderEngine.h>
#include <crsf/CoexistenceInterface/TPointMemoryObject.h>
#include <crsf/CoexistenceInterface/TDynamicStageMemory.h>
#include <crsf/CRModel/TWorld.h>

#include "openvr_camera_object.h"

CRSEEDLIB_MODULE_CREATOR(OpenVRModule);

// ************************************************************************************************
static const LMatrix4f z_to_y = LMatrix4f::convert_mat(CS_zup_right, CS_yup_right);
static const LMatrix4f y_to_z = LMatrix4f::convert_mat(CS_yup_right, CS_zup_right);

static void convert_matrix_to_lmatrix(const vr::HmdMatrix34_t& from, LMatrix4f& to)
{
	to = LMatrix4f(
		from.m[0][0], from.m[1][0], from.m[2][0], 0.0,
		from.m[0][1], from.m[1][1], from.m[2][1], 0.0,
		from.m[0][2], from.m[1][2], from.m[2][2], 0.0,
		from.m[0][3], from.m[1][3], from.m[2][3], 1.0f
	);
}

// ************************************************************************************************
struct OpenVRModule::Impl
{
	AsyncTask::DoneStatus update(rppanda::FunctionalTask*);

	Impl(OpenVRModule& self);

	void OnLoad(void);
	void OnExit(void);

public:
	OpenVRModule& self_;

	rpplugins::OpenVRPlugin* plugin_;

	rppanda::FunctionalTask* update_task_ = nullptr;
	double camera_timing_point_ = 0;

	bool initialized_ = false;
	crsf::TPointMemoryObject* device_point_memory_ = nullptr;

	std::shared_ptr<OpenVRCameraObject> camera_;
};

AsyncTask::DoneStatus OpenVRModule::Impl::update(rppanda::FunctionalTask* task)
{
	LMatrix4 pose;

	crsf::TPoint point;
	for (vr::TrackedDeviceIndex_t device_index = 0; device_index < vr::k_unMaxTrackedDeviceCount; ++device_index)
	{
		if (!self_.GetHMDInstance()->IsTrackedDeviceConnected(device_index))
			continue;

		if (!plugin_->get_tracked_device_pose(device_index).bPoseIsValid)
			continue;

		convert_matrix_to_lmatrix(plugin_->get_tracked_device_pose(device_index).mDeviceToAbsoluteTracking, pose);

		point.m_nIndex = device_index;
		point.m_Pose.SetMatrix(z_to_y * pose * y_to_z);

		device_point_memory_->SetPointMemory(device_index, point);
	}

	static double last_time = rpcore::Globals::clock->get_long_time();
	double current_time = rpcore::Globals::clock->get_long_time();
	if (current_time - last_time > 1.0 / 60.0)
	{
		device_point_memory_->UpdatePointMemoryObject();
		last_time = current_time;
	}

	return AsyncTask::DS_cont;
}

OpenVRModule::Impl::Impl(OpenVRModule& self): self_(self)
{
	auto plugin_mgr = crsf::TGraphicRenderEngine::GetInstance()->GetRenderPipeline()->get_plugin_mgr();

	if (!plugin_mgr->is_plugin_enabled("openvr"))
	{
		self_.m_logger->error("Not enabled OpenVR plugin in Render Pipeline.");
		throw std::runtime_error("OpenVR plugin is NOT enabled.");
	}
}

void OpenVRModule::Impl::OnLoad(void)
{
	self_.m_logger->trace("Initializing OpenVRModule.");

	auto plugin_mgr = crsf::TGraphicRenderEngine::GetInstance()->GetRenderPipeline()->get_plugin_mgr();
	plugin_ = static_cast<rpplugins::OpenVRPlugin*>(plugin_mgr->get_instance("openvr")->downcast());
	plugin_->set_distance_scale(crsf::TGraphicRenderEngine::GetInstance()->GetRenderingScaleFactor());

	crsf::TCRProperty prop;
	prop.m_strName = "OpenVRPoint";
	prop.m_propPoint.SetPointNumber(vr::k_unMaxTrackedDeviceCount);

	device_point_memory_ = crsf::TDynamicStageMemory::GetInstance()->CreatePointMemoryObject(prop);

	update_task_ = rpcore::Globals::base->add_task(
		std::bind(&Impl::update, this, std::placeholders::_1),
		"OpenVRModule::update_point_memory_object", -50);

	if (self_.m_property.get("use_camera", false))
		self_.CreateCamera();

	self_.m_logger->info("OpenVRModule was initialized.");
}

void OpenVRModule::Impl::OnExit(void)
{
	self_.m_logger->trace("Exiting OpenVRModule.");

	if (update_task_)
		rpcore::Globals::base->remove_task(update_task_->get_name());

	plugin_ = nullptr;
	device_point_memory_ = nullptr;
	update_task_ = nullptr;
	camera_ = nullptr;
}

// ************************************************************************************************
OpenVRModule::OpenVRModule(void): TDynamicModuleInterface(CRMODULE_ID_STRING), impl_(std::make_unique<Impl>(*this))
{
}

void OpenVRModule::OnLoad(void)
{
	impl_->OnLoad();
}

void OpenVRModule::OnStart(void)
{
}

void OpenVRModule::OnExit(void)
{
	impl_->OnExit();
}

LVecBase3f OpenVRModule::GetDevicePosition(int index) const
{
	if (index >= vr::k_unMaxTrackedDeviceCount)
	{
		m_logger->error("The index is over the max count of tracked devices: {}", index);
		return LVecBase3f(0);
	}

	return impl_->device_point_memory_->GetPointMemory(index).m_Pose.GetPosition();
}

LQuaternionf OpenVRModule::GetDeviceOrientation(int index) const
{
	if (index >= vr::k_unMaxTrackedDeviceCount)
	{
		m_logger->error("The index is over the max count of tracked devices: {}", index);
		return LQuaternionf::ident_quat();
	}

	return impl_->device_point_memory_->GetPointMemory(index).m_Pose.GetQuaternion();
}

LVecBase3f OpenVRModule::GetHMDPosition(void) const
{
	return GetDevicePosition(vr::k_unTrackedDeviceIndex_Hmd);
}

LQuaternionf OpenVRModule::GetHMDOrientation(void) const
{
	return GetDeviceOrientation(vr::k_unTrackedDeviceIndex_Hmd);
}

bool OpenVRModule::IsDeviceConnected(int index) const
{
	return GetHMDInstance()->IsTrackedDeviceConnected(index);
}

bool OpenVRModule::IsHMD(int index) const
{
	return GetHMDInstance()->GetTrackedDeviceClass(index) == vr::TrackedDeviceClass_HMD;
}

bool OpenVRModule::IsTrackerReference(int index) const
{
	return GetHMDInstance()->GetTrackedDeviceClass(index) == vr::TrackedDeviceClass_TrackingReference;
}

bool OpenVRModule::IsController(int index) const
{
	return GetHMDInstance()->GetTrackedDeviceClass(index) == vr::TrackedDeviceClass_Controller;
}

bool OpenVRModule::IsTracker(int index) const
{
	return GetHMDInstance()->GetTrackedDeviceClass(index) == vr::TrackedDeviceClass_GenericTracker;
}

int OpenVRModule::GetMaximumDeviceCount(void) const
{
	return vr::k_unMaxTrackedDeviceCount;
}

vr::IVRSystem* OpenVRModule::GetHMDInstance(void) const
{
	return impl_->plugin_->get_vr_system();
}

rpplugins::OpenVRPlugin* OpenVRModule::GetOpenVRPlugin(void) const
{
	return impl_->plugin_;
}

crsf::TPointMemoryObject* OpenVRModule::GetPointMemoryObject(void) const
{
	return impl_->device_point_memory_;
}

bool OpenVRModule::HasCamera(void) const
{
	return impl_->plugin_->has_tracked_camera();
}

OpenVRCameraObject* OpenVRModule::GetCamera(void) const
{
	return impl_->camera_.get();
}

OpenVRCameraObject* OpenVRModule::CreateCamera()
{
	if (impl_->camera_)
		return GetCamera();

	if (!HasCamera())
	{
		m_logger->error("Device has no camera!");
		return nullptr;
	}

	impl_->camera_ = crsf::CreateObject<OpenVRCameraObject>(*this);
	crsf::TGraphicRenderEngine::GetInstance()->GetWorld()->AddWorldObject(impl_->camera_);

	return GetCamera();
}
