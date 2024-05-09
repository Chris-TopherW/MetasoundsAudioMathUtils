// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundGateNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_GateNode"

namespace Metasound
{
	namespace GateNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameGateToggle, "Gate", "The value to test the input against")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FGateOperator
	//------------------------------------------------------------------------------------
	FGateOperator::FGateOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InGateToggle)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInGateToggle(InGateToggle)
	{

	}

	const FNodeClassMetadata& FGateOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Gate (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_GateDisplayName", "Gate (Audio)");
				Info.Description = LOCTEXT("Metasound_GateNodeDescription", "Passes input 1 through if input 2 does not round to 0, does not if input 2 rounds to 0");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_GateNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FGateOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace GateNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameGateToggle), mInGateToggle);
	}

	void FGateOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace GateNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FGateOperator::GetVertexInterface()
	{
		using namespace GateNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameGateToggle))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FGateOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace GateNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InGateToggle = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameGateToggle), InParams.OperatorSettings);


		return MakeUnique<FGateOperator>(InParams.OperatorSettings, AudioIn, InGateToggle);
	}

	void FGateOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputGateToggle = mInGateToggle->GetData();

		const int32 NumSamples = AudioInput->Num();

		GateDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputGateToggle, NumSamples);
	}

	METASOUND_REGISTER_NODE(FGateNode)
}

#undef LOCTEXT_NAMESPACE
