// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundWrapNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_WrapNode"

namespace Metasound
{
	namespace WrapNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FWrapOperator
	//------------------------------------------------------------------------------------
	FWrapOperator::FWrapOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FWrapOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Wrap (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_WrapDisplayName", "Wrap (Audio)");
				Info.Description = LOCTEXT("Metasound_WrapNodeDescription", "Applies Wrap to the audio input.");
				Info.Author = PluginAuthor;
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_WrapNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FWrapOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace WrapNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
	}

	void FWrapOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace WrapNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FWrapOperator::GetVertexInterface()
	{
		using namespace WrapNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput))),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FWrapOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace WrapNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);

		return MakeUnique<FWrapOperator>(InParams.OperatorSettings, AudioIn);
	}

	void FWrapOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		WrapDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FWrapNode)
}

#undef LOCTEXT_NAMESPACE
