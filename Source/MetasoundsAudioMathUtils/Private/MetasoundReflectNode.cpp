// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundReflectNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_ReflectNode"

namespace Metasound
{
	namespace ReflectNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FReflectOperator
	//------------------------------------------------------------------------------------
	FReflectOperator::FReflectOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FReflectOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Reflect (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_ReflectDisplayName", "Reflect (Audio)");
				Info.Description = LOCTEXT("Metasound_ReflectNodeDescription", "Applies Reflect to the audio input.");
				Info.Author = "geekrelief";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_ReflectNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FReflectOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace ReflectNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
	}

	void FReflectOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace ReflectNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FReflectOperator::GetVertexInterface()
	{
		using namespace ReflectNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput))),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FReflectOperator::CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors)
	{
		using namespace ReflectNode;

		const FInputVertexInterfaceData& InputInterface = InParams.InputData;

		FAudioBufferReadRef AudioIn = InputInterface.GetOrCreateDefaultDataReadReference<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);

		return MakeUnique<FReflectOperator>(InParams.OperatorSettings, AudioIn);
	}

	void FReflectOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		ReflectDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}


	FNodeClassMetadata FReflectNode::CreateNodeClassMetadata()
	{
		return FReflectOperator::GetNodeInfo();
	}

	METASOUND_REGISTER_NODE(FReflectNode)
}

#undef LOCTEXT_NAMESPACE
