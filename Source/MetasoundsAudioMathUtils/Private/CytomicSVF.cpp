// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

// Code based on this algorithm: https://gist.github.com/hollance/2891d89c57adc71d9560bcf0e1e55c4b

#include "CytomicSVF.h"
#include "Math/UnrealMathUtility.h"
#include "DSP/Dsp.h"

namespace DSPProcessing
{

CytomicSVF::CytomicSVF()
{
	SetLPF();
}

void CytomicSVF::SetLPF()
{
	m0 = 0.0f;
	m1 = 0.0f;
	m2 = 1.0f;
}

// currently feeding back so not exposed to MS node
void CytomicSVF::setHPF()
{
	m0 = 1.0f;
	m1 = -1.0f;
	m2 = -1.0f;
}

void CytomicSVF::setBP()
{
	m0 = 0.0f;
	m1 = 1.0f;
	m2 = 0.0f;
}

void CytomicSVF::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* CutoffFreq, const float* QAmount, const int32 InNumSamples)
{
	for (int i = 0; i < InNumSamples; ++i)
	{
		//calc coefficients
		g = Audio::FastTan(PI * CutoffFreq[i] / fs);
		k = 1.0f / QAmount[i];
		a1 = 1.0f / (1.0f + g * (g + k));
		a2 = g * a1;
		a3 = g * a2;

		float v3 = InBuffer[i] - ic2eq;
		float v1 = a1 * ic1eq + a2 * v3;
		float v2 = ic2eq + a2 * ic1eq + a3 * v3;
		ic1eq = 2.0f * v1 - ic1eq;
		ic2eq = 2.0f * v2 - ic2eq;

		OutBuffer[i] = m0 * OutBuffer[i] + m1 * k * v1 + m2 * v2;
	}
}

} //namespace DSPProcessing