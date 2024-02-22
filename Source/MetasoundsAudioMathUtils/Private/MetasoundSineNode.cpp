#include "MetasoundSineNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_SineNode"

namespace Metasound
{
	namespace SineNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FSineOperator
	//------------------------------------------------------------------------------------
	FSineOperator::FSineOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{

	}

	const FNodeClassMetadata& FSineOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Sine (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_SineDisplayName", "Sine (Audio)");
				Info.Description = LOCTEXT("Metasound_SineNodeDescription", "Applies Sine(2PI(Input))");
				Info.Author = PluginAuthor;
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_SineNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FSineOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace SineNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
	}

	void FSineOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace SineNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FSineOperator::GetVertexInterface()
	{
		using namespace SineNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput))),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FSineOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace SineNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);

		return MakeUnique<FSineOperator>(InParams.OperatorSettings, AudioIn);
	}

	void FSineOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		SineDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
	}

	METASOUND_REGISTER_NODE(FSineNode)
}

#undef LOCTEXT_NAMESPACE
