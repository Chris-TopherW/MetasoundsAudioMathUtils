// Fill out your copyright notice in the Description page of Project Settings.


#include "MetasoundInterpToAudioNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_InterpToAudioNode"

namespace Metasound
{
	namespace InterpToAudioNode
	{
		// Input params
		METASOUND_PARAM(InParamNameTargetValue, "Target", "Target Value.")
			METASOUND_PARAM(InParamNameTimeForInterpolation, "Time", "The amount of time for the interpolation.")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FInterpToAudioOperator
	//------------------------------------------------------------------------------------
	FInterpToAudioOperator::FInterpToAudioOperator(const FOperatorSettings& InSettings, const FFloatReadRef& targetValue, const FTimeReadRef& timeForInterpolation)
		: AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mTargetValue(targetValue)
		, mTimeForInterpolation(timeForInterpolation)
	{
		InterpToAudioDSPProcessor.Init(InSettings.GetSampleRate());
	}

	const FNodeClassMetadata& FInterpToAudioOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("InterpToAudio"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_InterpToAudioDisplayName", "InterpTo (Audio)");
				Info.Description = LOCTEXT("Metasound_InterpToAudioNodeDescription", "Audio rate interpolation to supplied value.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_InterpToAudioNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FInterpToAudioOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace InterpToAudioNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameTargetValue), mTargetValue);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameTimeForInterpolation), mTimeForInterpolation);
	}

	void FInterpToAudioOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace InterpToAudioNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FInterpToAudioOperator::GetVertexInterface()
	{
		using namespace InterpToAudioNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameTargetValue), 0.0f),
				TInputDataVertex<FTime>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameTimeForInterpolation), 1.0f)
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FInterpToAudioOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace InterpToAudioNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();


		FFloatReadRef targetValue = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameTargetValue), InParams.OperatorSettings);
		FTimeReadRef timeForInterpolation = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FTime>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameTimeForInterpolation), InParams.OperatorSettings);

		return MakeUnique<FInterpToAudioOperator>(InParams.OperatorSettings, targetValue, timeForInterpolation);
	}

	void FInterpToAudioOperator::Execute()
	{
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioOutput->Num();

		InterpToAudioDSPProcessor.SetTargetValue(*mTargetValue);
		InterpToAudioDSPProcessor.SetInterpTime((float)mTimeForInterpolation->GetSeconds());
		InterpToAudioDSPProcessor.ProcessAudioBuffer(OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FInterpToAudioNode)
}

#undef LOCTEXT_NAMESPACE
