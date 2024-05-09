// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundCosNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_CosNode"

namespace Metasound
{
	namespace CosNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FCosOperator
	//------------------------------------------------------------------------------------
	FCosOperator::FCosOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FCosOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Cos (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_CosDisplayName", "Cos (Audio)");
				Info.Description = LOCTEXT("Metasound_CosNodeDescription", "Applies Cos(2PI(Input))");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_CosNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FCosOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace CosNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
	}

	void FCosOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace CosNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FCosOperator::GetVertexInterface()
	{
		using namespace CosNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput))),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FCosOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace CosNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);

		return MakeUnique<FCosOperator>(InParams.OperatorSettings, AudioIn);
	}

	void FCosOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		CosDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FCosNode)
}

#undef LOCTEXT_NAMESPACE
