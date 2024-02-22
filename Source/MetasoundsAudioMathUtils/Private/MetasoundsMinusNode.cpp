#include "MetasoundsMinusNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_MinusNode"

namespace Metasound
{
	namespace MinusNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameMinusComparator, "Minus", "The value to test the input against")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FMinusOperator
	//------------------------------------------------------------------------------------
	FMinusOperator::FMinusOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InMinusComparator)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInMinusComparator(InMinusComparator)
	{

	}

	const FNodeClassMetadata& FMinusOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Minus (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_MinusDisplayName", "Minus (Audio)");
				Info.Description = LOCTEXT("Metasound_MinusNodeDescription", "Compares inputs, outputs 1 if the same, 0 if different.");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_MinusNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FMinusOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace MinusNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameMinusComparator), mInMinusComparator);
	}

	void FMinusOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace MinusNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FMinusOperator::GetVertexInterface()
	{
		using namespace MinusNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameMinusComparator))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FMinusOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace MinusNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InMinusComparator = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameMinusComparator), InParams.OperatorSettings);


		return MakeUnique<FMinusOperator>(InParams.OperatorSettings, AudioIn, InMinusComparator);
	}

	void FMinusOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputMinusComparator = mInMinusComparator->GetData();

		const int32 NumSamples = AudioInput->Num();

		MinusDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputMinusComparator, NumSamples);
	}

	METASOUND_REGISTER_NODE(FMinusNode)
}

#undef LOCTEXT_NAMESPACE
