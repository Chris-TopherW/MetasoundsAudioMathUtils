// Fill out your copyright notice in the Description page of Project Settings.


#include "MetasoundOnePoleIIRNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_OnePoleIIRNode"

namespace Metasound
{
	namespace OnePoleIIRNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameCoefficientA, "CoefA", "Multiplier for IIR feedback path")
			METASOUND_PARAM(InParamNameCoefficientB, "CoefB", "Multiplier for IIR current input sample")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FOnePoleIIROperator
	//------------------------------------------------------------------------------------
	FOnePoleIIROperator::FOnePoleIIROperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InCoefficientA, const FAudioBufferReadRef& InCoefficientB)
		: AudioInput(InAudioInput)
		, mCoefficientA(InCoefficientA)
		, mCoefficientB(InCoefficientB)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FOnePoleIIROperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("OnePoleIIR"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_OnePoleIIRDisplayName", "OnePoleIIR");
				Info.Description = LOCTEXT("Metasound_OnePoleIIRNodeDescription", "Applies yn = b*xn + a*yn-1 to the audio input.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_OnePoleIIRNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FOnePoleIIROperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace OnePoleIIRNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameCoefficientA), mCoefficientA);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameCoefficientB), mCoefficientB);
	}

	void FOnePoleIIROperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace OnePoleIIRNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FOnePoleIIROperator::GetVertexInterface()
	{
		using namespace OnePoleIIRNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameCoefficientA)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameCoefficientB))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FOnePoleIIROperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace OnePoleIIRNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InCoefficientA = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameCoefficientA), InParams.OperatorSettings);
		FAudioBufferReadRef InCoefficientB = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameCoefficientB), InParams.OperatorSettings);

		return MakeUnique<FOnePoleIIROperator>(InParams.OperatorSettings, AudioIn, InCoefficientA, InCoefficientB);
	}

	void FOnePoleIIROperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		const float* CoefA = mCoefficientA->GetData();
		const float* CoefB = mCoefficientB->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		OnePoleIIRDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, CoefA, CoefB, NumSamples);
	}

	METASOUND_REGISTER_NODE(FOnePoleIIRNode)
}

#undef LOCTEXT_NAMESPACE
