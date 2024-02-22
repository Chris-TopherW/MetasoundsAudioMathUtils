#include "MetasoundLessThanNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_LessThanNode"

namespace Metasound
{
	namespace LessThanNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameLessThanComparator, "LessThan", "The value to test the input against")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FLessThanOperator
	//------------------------------------------------------------------------------------
	FLessThanOperator::FLessThanOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InLessThanComparator)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInLessThanComparator(InLessThanComparator)
	{

	}

	const FNodeClassMetadata& FLessThanOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("LessThan (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_LessThanDisplayName", "LessThan (Audio)");
				Info.Description = LOCTEXT("Metasound_LessThanNodeDescription", "Passes 1 through if input 1 is Less than input 2, otherwise passes 0");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_LessThanNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FLessThanOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace LessThanNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameLessThanComparator), mInLessThanComparator);
	}

	void FLessThanOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace LessThanNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FLessThanOperator::GetVertexInterface()
	{
		using namespace LessThanNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameLessThanComparator))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FLessThanOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace LessThanNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InLessThanComparator = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameLessThanComparator), InParams.OperatorSettings);


		return MakeUnique<FLessThanOperator>(InParams.OperatorSettings, AudioIn, InLessThanComparator);
	}

	void FLessThanOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputLessThanComparator = mInLessThanComparator->GetData();

		const int32 NumSamples = AudioInput->Num();

		LessThanDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputLessThanComparator, NumSamples);
	}

	METASOUND_REGISTER_NODE(FLessThanNode)
}

#undef LOCTEXT_NAMESPACE
