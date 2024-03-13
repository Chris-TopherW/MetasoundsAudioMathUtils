// Fill out your copyright notice in the Description page of Project Settings.


#include "MetasoundOnePoleFIRNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_OnePoleFIRNode"

namespace Metasound
{
	namespace OnePoleFIRNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameCoefficientA, "CoefA", "Multiplier for FIR delay path")
			METASOUND_PARAM(InParamNameCoefficientB, "CoefB", "Multiplier for FIR current input sample")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FOnePoleFIROperator
	//------------------------------------------------------------------------------------
	FOnePoleFIROperator::FOnePoleFIROperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InCoefficientA, const FAudioBufferReadRef& InCoefficientB)
		: AudioInput(InAudioInput)
		, mCoefficientA(InCoefficientA)
		, mCoefficientB(InCoefficientB)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FOnePoleFIROperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("OnePoleFIR"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_OnePoleFIRDisplayName", "OnePoleFIR");
				Info.Description = LOCTEXT("Metasound_OnePoleFIRNodeDescription", "Applies yn = b*xn + a*xn-1 to the audio input.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_OnePoleFIRNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FOnePoleFIROperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace OnePoleFIRNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameCoefficientA), mCoefficientA);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameCoefficientB), mCoefficientB);
	}

	void FOnePoleFIROperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace OnePoleFIRNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FOnePoleFIROperator::GetVertexInterface()
	{
		using namespace OnePoleFIRNode;

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

	TUniquePtr<IOperator> FOnePoleFIROperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace OnePoleFIRNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InCoefficientA = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameCoefficientA), InParams.OperatorSettings);
		FAudioBufferReadRef InCoefficientB = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameCoefficientB), InParams.OperatorSettings);

		return MakeUnique<FOnePoleFIROperator>(InParams.OperatorSettings, AudioIn, InCoefficientA, InCoefficientB);
	}

	void FOnePoleFIROperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		const float* CoefA = mCoefficientA->GetData();
		const float* CoefB = mCoefficientB->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		OnePoleFIRDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, CoefA, CoefB, NumSamples);
	}

	METASOUND_REGISTER_NODE(FOnePoleFIRNode)
}

#undef LOCTEXT_NAMESPACE
