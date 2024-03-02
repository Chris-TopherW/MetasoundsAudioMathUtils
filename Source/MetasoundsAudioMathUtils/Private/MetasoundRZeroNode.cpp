// Fill out your copyright notice in the Description page of Project Settings.


#include "MetasoundRZeroNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_RZeroNode"

namespace Metasound
{
	namespace RZeroNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameCoefficient, "Coefficient", "The amount of Coefficient to apply to the input signal.")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FRZeroOperator
	//------------------------------------------------------------------------------------
	FRZeroOperator::FRZeroOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InCoefficient)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mCoefficient(InCoefficient)
	{

	}

	const FNodeClassMetadata& FRZeroOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("RZero"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_RZeroDisplayName", "RZero");
				Info.Description = LOCTEXT("Metasound_RZeroNodeDescription", "Applies yn = xn - an * yn-1 to the audio input.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_RZeroNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FRZeroOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace RZeroNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameCoefficient), mCoefficient);
	}

	void FRZeroOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace RZeroNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FRZeroOperator::GetVertexInterface()
	{
		using namespace RZeroNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameCoefficient), 1.0f)
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FRZeroOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace RZeroNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FFloatReadRef InCoefficient = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameCoefficient), InParams.OperatorSettings);

		return MakeUnique<FRZeroOperator>(InParams.OperatorSettings, AudioIn, InCoefficient);
	}

	void FRZeroOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		RZeroDSPProcessor.SetCoef(*mCoefficient);
		RZeroDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FRZeroNode)
}

#undef LOCTEXT_NAMESPACE
