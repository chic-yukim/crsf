/**
 * Coexistence Reality Software Framework (CRSF)
 * Copyright (c) Center of Human-centered Interaction for Coexistence. All rights reserved.
 * See the LICENSE.md file for more details.
 */

#pragma once

#include <memory>

#include <crsf/RealWorldInterface/TCRImageInterface.h>

namespace crsf {
class TImageMemoryObject;
}

class WebcamInterface: public crsf::TCRImageInterface
{
public:
	WebcamInterface(int nIndex);
	WebcamInterface(void);
	~WebcamInterface(void);

	bool Init(void) override;

	void Grab(crsf::TImageMemoryObject *pImageMemoryObject) override;

	void Exit(void) override;

private:
	struct Impl;
	std::unique_ptr<Impl> impl_;
};
