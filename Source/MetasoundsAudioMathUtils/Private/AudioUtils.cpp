// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioUtils.h"
#include "Math/UnrealMathUtility.h"

namespace DSPProcessing
{

void FCos::Init() {}

void FCos::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = FMath::Cos((2.0f * PI) * InBuffer[Index]);
	}
}

void FEquals::Init() {}

void FEquals::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputEqualsComparator, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] == InputEqualsComparator[Index] ? 1.0f : 0.0f;
	}
}

void FGate::Init() {}

void FGate::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGateToggle, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = FMath::RoundHalfFromZero(InputGateToggle[Index]) != 0.0f ? InBuffer[Index] : 0.0f;
	}
}

void FGreaterThan::Init() {}

void FGreaterThan::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGreaterThanComparator, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] > InputGreaterThanComparator[Index] ? 1.0f : 0.0f;
	}
}

void FInterpToAudio::Init(float sampleRate) 
{
	mSampleRate = sampleRate;
}

void FInterpToAudio::SetTargetValue(const float targetValue)
{
	mTargetValue = targetValue;
	mLerpProgress = 0.0f;
	mLerpStartValue = currentValue;
}

void FInterpToAudio::SetInterpTime(const float time)
{
	mSamplesInLerp = static_cast<int>(time * mSampleRate);
}

void FInterpToAudio::ProcessAudioBuffer(float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		if (mLerpProgress >= 1.0f)
		{
			OutBuffer[Index] = mTargetValue;
		}
		else
		{
			currentValue = FMath::Lerp(mLerpStartValue, mTargetValue, mLerpProgress);
			OutBuffer[Index] = currentValue;
			mLerpProgress += 1.0f / static_cast<float>(mSamplesInLerp);
		}
	}
}

void FLessThan::Init() {}

void FLessThan::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputLessThanComparator, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] < InputLessThanComparator[Index] ? 1.0f : 0.0f;
	}
}

void FNotEquals::Init() {}

void FNotEquals::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputNotEqualsComparator, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] != InputNotEqualsComparator[Index] ? 1.0f : 0.0f;
	}
}

void FPow::Init() {}

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

void FOnePoleIIR::Init() {}

void FOnePoleIIR::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] * coefB[Index] + mPreviousSample * coefA[Index];
		mPreviousSample = OutBuffer[Index];
	}
}

void FOnePoleFIR::Init() {}

void FOnePoleFIR::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = InBuffer[Index] * coefB[Index] + mPreviousInputSample * coefA[Index];
		mPreviousInputSample = InBuffer[Index];
	}
}

void FSamphold::Init() {}

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

void FSine::Init() {}

void FSine::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = FMath::Sin((2.0f * PI) * InBuffer[Index]);
	}
}

void FSqrt::Init() {}

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

void FWrap::Init() {}

void FWrap::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples)
{
	for (int32 Index = 0; Index < InNumSamples; ++Index)
	{
		OutBuffer[Index] = sWrap(InBuffer[Index]);
	}
}

} // namespace DSPProcessing
