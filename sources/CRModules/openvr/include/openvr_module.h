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

#include <crsf/CRAPI/TDynamicModuleInterface.h>

#include <openvr.h>

namespace crsf {
class TPointMemoryObject;
}

namespace rpplugins {
class OpenVRPlugin;
}

class OpenVRCameraObject;

class OpenVRModule: public crsf::TDynamicModuleInterface
{
public:
	OpenVRModule(void);

	void OnLoad(void) override;
	void OnStart(void) override;
	void OnExit(void) override;

	virtual crsf::TPointMemoryObject* GetPointMemoryObject(void) const;

	virtual LVecBase3 GetDevicePosition(int index) const;
	virtual LQuaternion GetDeviceOrientation(int index) const;

	/** Get a pose of HMD in meter. */
	virtual LVecBase3f GetHMDPosition(void) const;
	virtual LQuaternionf GetHMDOrientation(void) const;

	virtual bool IsDeviceConnected(int index) const;

	virtual bool IsHMD(int index) const;
	virtual bool IsTrackerReference(int index) const;
	virtual bool IsController(int index) const;
	virtual bool IsTracker(int index) const;

	virtual int GetMaximumDeviceCount(void) const;

	virtual vr::IVRSystem* GetHMDInstance(void) const;
	virtual rpplugins::OpenVRPlugin* GetOpenVRPlugin(void) const;

	virtual bool HasCamera(void) const;
	virtual OpenVRCameraObject* GetCamera(void) const;
	virtual OpenVRCameraObject* CreateCamera();

private:
	friend class OpenVRCameraObject;

	struct Impl;
	std::unique_ptr<Impl> impl_;
};
