// Fill out your copyright notice in the Description page of Project Settings.

#include "CytomicSVF.h"
#include "Math/UnrealMathUtility.h"
#include "DSP/Dsp.h"

namespace DSPProcessing
{

void CytomicSVF::reset() noexcept
{
	ic1eq = 0.0f;
	ic2eq = 0.0f;
}

void CytomicSVF::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* CutoffFreq, const float* QAmount, const int32 InNumSamples) noexcept
{
	for (int i = 0; i < InNumSamples; ++i)
	{
		//calc coefficients
		g = Audio::FastTan(PI * CutoffFreq[i] / fs);
		a1 = 1.0f / (1.0f + g * (g + 1.0f / QAmount[i]));
		a2 = g * a1;
		a3 = g * a2;

		float v3 = InBuffer[i] - ic2eq;
		float v1 = a1 * ic1eq + a2 * v3;
		float v2 = ic2eq + a2 * ic1eq + a3 * v3;
		ic1eq = 2.0f * v1 - ic1eq;
		ic2eq = 2.0f * v2 - ic2eq;
		OutBuffer[i] = v2;
	}
}

} //namespace DSPProcessing