// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Platform.h"

/**
 * 
 */
class METASOUNDSAUDIOMATHUTILS_API AudioUtils
{
public:
	AudioUtils();
	~AudioUtils();
};

namespace DSPProcessing
{

class FCos
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};


class FEquals
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputEqualsComparator, const int32 InNumSamples);
};

class FGate
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGateToggle, const int32 InNumSamples);
};

class FGreaterThan
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputGreaterThanComparator, const int32 InNumSamples);
};

class FLessThan
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputLessThanComparator, const int32 InNumSamples);
};

class FNotEquals
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputNotEqualsComparator, const int32 InNumSamples);
};

class FPow
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputPowerOf, const int32 InNumSamples);
};

class FSine
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const int32 InNumSamples);
};

class FMinus
{
public:
	void Init();
	void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, const float* InputMinus, const int32 InNumSamples);
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
