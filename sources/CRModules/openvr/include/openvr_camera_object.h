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

#include <render_pipeline/rppanda/task/functional_task.hpp>

#include <crsf/CRModel/TPhysicalCamera.h>

#include <openvr_module.h>

namespace rpplugins {
class OpenVRCameraInterface;
}

namespace crsf {
class TImageMemoryObject;
}

class OpenVRCameraObject: public crsf::TPhysicalCamera
{
public:
    static const char* UPDATE_EVENT_NAME;

public:
    /** Create new phsyical camera instance. */
    OpenVRCameraObject(OpenVRModule& module_instance);

    bool StartStream() override;
    void StopStream() override;

    /**
     * Get current frame header.
     *
     * You can use "nFrameSequence" to check if the frame is updated, or not.
     */
    const vr::CameraVideoStreamFrameHeader_t& GetFrameHeader() const;

private:
    AsyncTask::DoneStatus Update(rppanda::FunctionalTask*);

    OpenVRModule& module_;
    rpplugins::OpenVRCameraInterface* camera_;
    std::vector<uint8_t> framebuffer_;
    vr::CameraVideoStreamFrameHeader_t frame_header_;
    rppanda::FunctionalTask* update_task_;
    double last_task_time_ = 0;
};

// ************************************************************************************************

inline const vr::CameraVideoStreamFrameHeader_t& OpenVRCameraObject::GetFrameHeader() const
{
    return frame_header_;
}
