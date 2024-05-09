// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "CoreMinimal.h"

namespace DSPProcessing
{

class METASOUNDSAUDIOMATHUTILS_API CytomicSVF
{
public:
	CytomicSVF();
	void SetLPF();
	void setHPF();
	void setBP();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* CutoffFreq, const float* QAmount, const int32 InNumSamples);

private:
	float g = 0.0f, k = 0.0f, a1 = 0.0f, a2 = 0.0f, a3 = 0.0f;  // filter coefficients
	float m0 = 0.0f, m1 = 0.0f, m2 = 0.0f;
	float ic1eq = 0.0f, ic2eq = 0.0f;      // internal state
	const float fs = 48000.0f;
};

} //namespace DSPProcessing
