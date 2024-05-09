// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/


#include "AudioUtils.h"
#include "Math/UnrealMathUtility.h"

namespace DSPProcessing
{
	void FAudioDivide::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputAudioDivide, const int32 InNumSamples)
	{
		for (int32 Index = 0; Index < InNumSamples; ++Index)
		{
			// protect against divide by zero
			OutBuffer[Index] = InputAudioDivide[Index] == 0.0f ? 0.0f : InBuffer[Index] / InputAudioDivide[Index];
		}
	}

void FCos::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = FMath::Cos((2.0f * PI) * InBuffer[Index]);
	}
}

void FGate::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGateToggle, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = FMath::RoundHalfFromZero(InputGateToggle[Index]) != 0.0f ? InBuffer[Index] : 0.0f;
	}
}

void FPow::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputPowerOf, const int32 InNumSamples)
{
	float inVal, powVal = 0.0f;
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		inVal = InBuffer[Index];
		powVal = InputPowerOf[Index];
		if (inVal < 0.0f)
		{
			OutBuffer[Index] = -1 * FMath::Pow(FMath::Abs(inVal), powVal);
		}
		else
		{
			OutBuffer[Index] = FMath::Pow(FMath::Abs(inVal), powVal);
		}
	}
}

void FOnePoleIIR::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] * coefB[Index] + mPreviousSample * coefA[Index];
		mPreviousSample = OutBuffer[Index];
	}
}

void FOnePoleFIR::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] * coefB[Index] + mPreviousInputSample * coefA[Index];
		mPreviousInputSample = InBuffer[Index];
	}
}

void FSamphold::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputSampholdPhasor, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		if (InputSampholdPhasor[Index] < mPreviousPhasorValue)
		{
			mHeldSample = InBuffer[Index];
		}

		OutBuffer[Index] = mHeldSample;
		mPreviousPhasorValue = InputSampholdPhasor[Index];
	}
}

void FSine::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = FMath::Sin((2.0f * PI) * InBuffer[Index]);
	}
}

void FSqrt::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = (InBuffer[Index] < 0 ? -1 : 1) * FMath::Sqrt(FMath::Abs(InBuffer[Index]));
	}
}

float sWrap(float input)
{
	while (input < -1.0f || input > 1.0f)
	{
		if (input > 1.0f)
		{
			input += ((input - 1.0) * -1.0f) * 2.0f;
		}
		else if ((input < -1.0f))
		{
			input += ((input + 1.0) * -1.0f) * 2.0f;
		}
	}

	return input;
}

void FWrap::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = sWrap(InBuffer[Index]);
	}
}

} // namespace DSPProcessing
