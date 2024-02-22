#include "MetasoundNotEqualsNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_NotEqualsNode"

namespace Metasound
{
	namespace NotEqualsNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameNotEqualsComparator, "NotEquals", "The value to test the input against")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FNotEqualsOperator
	//------------------------------------------------------------------------------------
	FNotEqualsOperator::FNotEqualsOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InNotEqualsComparator)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInNotEqualsComparator(InNotEqualsComparator)
	{

	}

	const FNodeClassMetadata& FNotEqualsOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("NotEquals (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_NotEqualsDisplayName", "NotEquals (Audio)");
				Info.Description = LOCTEXT("Metasound_NotEqualsNodeDescription", "Returns input 1 to the power of input 2");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_NotEqualsNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FNotEqualsOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace NotEqualsNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameNotEqualsComparator), mInNotEqualsComparator);
	}

	void FNotEqualsOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace NotEqualsNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FNotEqualsOperator::GetVertexInterface()
	{
		using namespace NotEqualsNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameNotEqualsComparator))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FNotEqualsOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace NotEqualsNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InNotEqualsComparator = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameNotEqualsComparator), InParams.OperatorSettings);


		return MakeUnique<FNotEqualsOperator>(InParams.OperatorSettings, AudioIn, InNotEqualsComparator);
	}

	void FNotEqualsOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputNotEqualsComparator = mInNotEqualsComparator->GetData();

		const int32 NumSamples = AudioInput->Num();

		NotEqualsDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputNotEqualsComparator, NumSamples);
	}

	METASOUND_REGISTER_NODE(FNotEqualsNode)
}

#undef LOCTEXT_NAMESPACE
