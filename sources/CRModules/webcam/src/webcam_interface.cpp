/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#include "webcam_interface.h"

#include <spdlog/logger.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <crsf/CoexistenceInterface/TDynamicStageMemory.h>
#include <crsf/CoexistenceInterface/TImageMemoryObject.h>

extern spdlog::logger* module_logger_;

struct WebcamInterface::Impl
{
	bool Init(void);
	void Grab(crsf::TImageMemoryObject *pImageMemoryObject);
	void Exit(void);

	cv::VideoCapture* m_pVideo_capture = nullptr;
	cv::Mat m_mat_frame;

	bool m_bSetFlag = false;
	int m_nIndex = 0;
};

bool WebcamInterface::Impl::Init(void)
{
	module_logger_->debug("Initializing WebcamInterface.");

	m_pVideo_capture = new cv::VideoCapture(m_nIndex);

	if (!m_pVideo_capture->isOpened())
	{
		module_logger_->error("Cannot open video capture.");

		delete m_pVideo_capture;
		m_pVideo_capture = nullptr;

		return false;
	}

	m_bSetFlag = false;

	return true;
}

void WebcamInterface::Impl::Grab(crsf::TImageMemoryObject *pImageMemoryObject)
{
	if (!m_bSetFlag)
	{
		m_pVideo_capture->set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
		m_pVideo_capture->set(CV_CAP_PROP_FRAME_WIDTH, pImageMemoryObject->GetProperty().m_propImage.GetWidth());
		m_pVideo_capture->set(CV_CAP_PROP_FRAME_HEIGHT, pImageMemoryObject->GetProperty().m_propImage.GetHeight());

		m_bSetFlag = true;
	}

	m_pVideo_capture->read(m_mat_frame);

	unsigned int nFrameSize = m_mat_frame.rows * m_mat_frame.cols * m_mat_frame.channels()*(sizeof(unsigned char));

	pImageMemoryObject->SetImageMemory(m_mat_frame.data, nFrameSize);
	pImageMemoryObject->UpdateImageMemoryObject();

	//cv::imshow("ASD", m_mat_frame);
	//cv::waitKey(10);
}

void WebcamInterface::Impl::Exit()
{
	module_logger_->debug("Exiting WebcamInterface.");

	m_pVideo_capture->release();
	delete m_pVideo_capture;
	m_pVideo_capture = nullptr;
}

// ************************************************************************************************

WebcamInterface::WebcamInterface(int nIndex): impl_(std::make_unique<Impl>())
{
	module_logger_->debug("Constructing WebcamInterface.");

	impl_->m_nIndex = nIndex;
}

WebcamInterface::WebcamInterface(void): WebcamInterface(0)
{
}

WebcamInterface::~WebcamInterface(void) = default;

bool WebcamInterface::Init()
{
	return impl_->Init();
}

void WebcamInterface::Grab(crsf::TImageMemoryObject *pImageMemoryObject)
{
	impl_->Grab(pImageMemoryObject);
}

void WebcamInterface::Exit()
{
	impl_->Exit();
}
