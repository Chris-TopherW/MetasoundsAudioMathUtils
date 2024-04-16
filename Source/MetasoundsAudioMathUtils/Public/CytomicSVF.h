// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace DSPProcessing
{

class METASOUNDSAUDIOMATHUTILS_API CytomicSVF
{
public:
	void reset() noexcept;
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* CutoffFreq, const float* QAmount, const int32 InNumSamples) noexcept;

private:
	float g = 0.0f, k = 0.0f, a1 = 0.0f, a2 = 0.0f, a3 = 0.0f;  // filter coefficients
	float ic1eq = 0.0f, ic2eq = 0.0f;      // internal state
	const float fs = 48000.0f;
};

} //namespace DSPProcessing
