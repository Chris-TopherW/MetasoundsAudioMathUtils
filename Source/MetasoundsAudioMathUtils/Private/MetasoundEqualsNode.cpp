#include "MetasoundEqualsNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_EqualsNode"

namespace Metasound
{
	namespace EqualsNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
		METASOUND_PARAM(InParamNameEqualsComparator, "Equals", "The value to test the input against")

		// Output params
		METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FEqualsOperator
	//------------------------------------------------------------------------------------
	FEqualsOperator::FEqualsOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InEqualsComparator)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInEqualsComparator(InEqualsComparator)
	{

	}

	const FNodeClassMetadata& FEqualsOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Equals (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_EqualsDisplayName", "Equals (Audio)");
				Info.Description = LOCTEXT("Metasound_EqualsNodeDescription", "Compares inputs, outputs 1 if the same, 0 if different.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_EqualsNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FEqualsOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace EqualsNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameEqualsComparator), mInEqualsComparator);
	}

	void FEqualsOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace EqualsNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FEqualsOperator::GetVertexInterface()
	{
		using namespace EqualsNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameEqualsComparator))
				),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FEqualsOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace EqualsNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InEqualsComparator = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameEqualsComparator), InParams.OperatorSettings);


		return MakeUnique<FEqualsOperator>(InParams.OperatorSettings, AudioIn, InEqualsComparator);
	}

	void FEqualsOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputEqualsComparator = mInEqualsComparator->GetData();

		const int32 NumSamples = AudioInput->Num();

		EqualsDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputEqualsComparator, NumSamples);
	}

	METASOUND_REGISTER_NODE(FEqualsNode)
}

#undef LOCTEXT_NAMESPACE
