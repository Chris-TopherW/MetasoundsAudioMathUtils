// Fill out your copyright notice in the Description page of Project Settings.


#include "MetasoundRawFIRNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_RawFIRNode"

namespace Metasound
{
	namespace RawFIRNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameCoefficient, "Coefficient", "The amount of Coefficient to apply to the input signal.")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FRawFIROperator
	//------------------------------------------------------------------------------------
	FRawFIROperator::FRawFIROperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InCoefficient)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mCoefficient(InCoefficient)
	{

	}

	const FNodeClassMetadata& FRawFIROperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("RawFIR"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_RawFIRDisplayName", "RawFIR");
				Info.Description = LOCTEXT("Metasound_RawFIRNodeDescription", "Applies yn = xn + an * yn-1 to the audio input.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_RawFIRNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FRawFIROperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace RawFIRNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameCoefficient), mCoefficient);
	}

	void FRawFIROperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace RawFIRNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FRawFIROperator::GetVertexInterface()
	{
		using namespace RawFIRNode;

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

	TUniquePtr<IOperator> FRawFIROperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace RawFIRNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FFloatReadRef InCoefficient = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameCoefficient), InParams.OperatorSettings);

		return MakeUnique<FRawFIROperator>(InParams.OperatorSettings, AudioIn, InCoefficient);
	}

	void FRawFIROperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		RawFIRDSPProcessor.SetCoef(*mCoefficient);
		RawFIRDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FRawFIRNode)
}

#undef LOCTEXT_NAMESPACE
