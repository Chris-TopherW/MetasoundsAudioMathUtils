// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundSqrtNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_SqrtNode"

namespace Metasound
{
	namespace SqrtNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FSqrtOperator
	//------------------------------------------------------------------------------------
	FSqrtOperator::FSqrtOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FSqrtOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Sqrt"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_SqrtDisplayName", "Sqrt (Audio)");
				Info.Description = LOCTEXT("Metasound_SqrtNodeDescription", "Applies Sqrt to the audio input.");
				Info.Author = PluginAuthor;
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_SqrtNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FSqrtOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace SqrtNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
	}

	void FSqrtOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace SqrtNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FSqrtOperator::GetVertexInterface()
	{
		using namespace SqrtNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput))),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FSqrtOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace SqrtNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);

		return MakeUnique<FSqrtOperator>(InParams.OperatorSettings, AudioIn);
	}

	void FSqrtOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		SqrtDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FSqrtNode)
}

#undef LOCTEXT_NAMESPACE
