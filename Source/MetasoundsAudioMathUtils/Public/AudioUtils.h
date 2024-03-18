// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"

namespace DSPProcessing
{

class FCos
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FGate
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGateToggle, const int32 InNumSamples);
};

class FInterpToAudio
{
public:
	void Init(float sampleRate);
	void SetTargetValue(const float coef);
	void SetInterpTime(const float time);
	void ProcessAudioBuffer(float* OutBuffer, const int32 startFrame, const int32 endFrame);

private:
	float mTargetValue = 0.0f;
	float mLerpProgress = 0.0f;
	int mSamplesInLerp = 0;
	float mLerpStartValue = 0.0f;
	float currentValue = 0.0f;
	float mSampleRate = 0.0f;
};

class FPow
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputPowerOf, const int32 InNumSamples);
};

class FOnePoleIIR
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples);
private:
	float mPreviousSample = 0.0f;
};

class FOnePoleFIR
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* coefA, const float* coefB, const int32 InNumSamples);
private:
	float mPreviousInputSample = 0.0f;
};

class FSamphold
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputSampholdPhasor, const int32 InNumSamples);
private:
	float mHeldSample = 0.0f;
	float mPreviousPhasorValue = 0.0f;
};

class FSine
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FSqrt
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FWrap
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

} // namespace DSPProcessing
