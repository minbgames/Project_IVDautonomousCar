#pragma once

#include <iostream>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "ApiController.h"


class VimbaCam
{
private:
	AVT::VmbAPI::Examples::ApiController apiController;

public:
	VimbaCam() {};
	~VimbaCam() {};

	int start();
	int stopAcquisition();
};
