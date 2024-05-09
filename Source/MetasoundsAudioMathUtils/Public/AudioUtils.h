// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"

namespace DSPProcessing
{

	//constexpr float sinc100[] =
	//{0.0f, 0.0040394f, -0.0049956f, 0.00196776f, 0.00277942f, -0.00549833f, 0.00390557f, 0.000915266f, 
	//	-0.0052291f, 0.0055295f, -0.00137314f, -0.00411038f, 0.00655771f, -0.00382482f, -0.00216395f, 
	//	0.00674355f, -0.00611859f, 0.000480447f, 0.00590667f, -0.00790079f, 0.00358686f, 0.00395832f, 
	//	-0.00881646f, 0.00682457f, 0.00091773f, -0.00853862f, 0.0097864f, -0.00308269f, -0.00678929f, 
	//	0.0120079f, -0.00780443f, -0.00334221f, 0.0129758f, -0.0129218f, 0.00201072f, 0.0121007f, 
	//	-0.0180509f, 0.00957585f, 0.00858315f, -0.022786f, 0.0201139f, 0.000918963f, -0.026739f, 0.0361677f, 
	//	-0.0153096f, -0.0295782f, 0.0701196f, -0.0656615f, -0.0310615f, 0.441174f, 0.441174f, -0.0310615f, 
	//	-0.0656615f, 0.0701196f, -0.0295782f, -0.0153096f, 0.0361677f, -0.026739f, 0.000918963f, 0.0201139f, 
	//	-0.022786f, 0.00858315f, 0.00957585f, -0.0180509f, 0.0121007f, 0.00201072f, -0.0129218f, 0.0129758f, 
	//	-0.00334221f, -0.00780443f, 0.0120079f, -0.00678929f, -0.00308269f, 0.0097864f, -0.00853862f, 
	//	0.00091773f, 0.00682457f, -0.00881646f, 0.00395832f, 0.00358686f, -0.00790079f, 0.00590667f, 0.000480447 
	//	-0.00611859f, 0.00674355 -0.00216395 -0.00382482, 0.00655771 -0.00411038 -0.00137314,  
	//	0.0055295 -0.0052291, 0.000915266, 0.00390557f, -0.00549833f, 0.00277942f, 0.00196776f, -0.0049956f, 
	//	0.0040394f, 0.0f };

class FAudioDivide
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGateToggle, const int32 InNumSamples);
};

class FCos
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FGate
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGateToggle, const int32 InNumSamples);
};

class FPow
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputPowerOf, const int32 InNumSamples);
};

class FOnePoleIIR
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples);
private:
	float mPreviousSample = 0.0f;
};

class FOnePoleFIR
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples);
private:
	float mPreviousInputSample = 0.0f;
};

class FSamphold
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputSampholdPhasor, const int32 InNumSamples);
private:
	float mHeldSample = 0.0f;
	float mPreviousPhasorValue = 0.0f;
};

class FSine
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FSqrt
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FWrap
{
public:
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

} // namespace DSPProcessing
