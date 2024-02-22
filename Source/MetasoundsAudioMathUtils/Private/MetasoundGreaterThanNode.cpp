#include "MetasoundGreaterThanNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_GreaterThanNode"

namespace Metasound
{
	namespace GreaterThanNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameGreaterThanComparator, "GreaterThan", "The value to test the input against")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FGreaterThanOperator
	//------------------------------------------------------------------------------------
	FGreaterThanOperator::FGreaterThanOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InGreaterThanComparator)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInGreaterThanComparator(InGreaterThanComparator)
	{

	}

	const FNodeClassMetadata& FGreaterThanOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("GreaterThan (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_GreaterThanDisplayName", "GreaterThan (Audio)");
				Info.Description = LOCTEXT("Metasound_GreaterThanNodeDescription", "Passes 1 through if input 1 is greater than input 2, otherwise passes 0");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_GreaterThanNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FGreaterThanOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace GreaterThanNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameGreaterThanComparator), mInGreaterThanComparator);
	}

	void FGreaterThanOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace GreaterThanNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FGreaterThanOperator::GetVertexInterface()
	{
		using namespace GreaterThanNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameGreaterThanComparator))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FGreaterThanOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace GreaterThanNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InGreaterThanComparator = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameGreaterThanComparator), InParams.OperatorSettings);


		return MakeUnique<FGreaterThanOperator>(InParams.OperatorSettings, AudioIn, InGreaterThanComparator);
	}

	void FGreaterThanOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputGreaterThanComparator = mInGreaterThanComparator->GetData();

		const int32 NumSamples = AudioInput->Num();

		GreaterThanDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputGreaterThanComparator, NumSamples);
	}

	METASOUND_REGISTER_NODE(FGreaterThanNode)
}

#undef LOCTEXT_NAMESPACE
