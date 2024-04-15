#include "MetasoundAudioDivideNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_AudioDivideNode"

namespace Metasound
{
	namespace AudioDivideNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameAudioDivide, "AudioDivide", "The value to divide by")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FAudioDivideOperator
	//------------------------------------------------------------------------------------
	FAudioDivideOperator::FAudioDivideOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InAudioDivide)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInAudioDivide(InAudioDivide)
	{

	}

	const FNodeClassMetadata& FAudioDivideOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("AudioDivide (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_AudioDivideDisplayName", "AudioDivide (Audio)");
				Info.Description = LOCTEXT("Metasound_AudioDivideNodeDescription", "Divide input 1 by input 2. Divide by 0 locked to 0");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_AudioDivideNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FAudioDivideOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace AudioDivideNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioDivide), mInAudioDivide);
	}

	void FAudioDivideOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace AudioDivideNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FAudioDivideOperator::GetVertexInterface()
	{
		using namespace AudioDivideNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioDivide))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FAudioDivideOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace AudioDivideNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InAudioDivide = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioDivide), InParams.OperatorSettings);


		return MakeUnique<FAudioDivideOperator>(InParams.OperatorSettings, AudioIn, InAudioDivide);
	}

	void FAudioDivideOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputAudioDivide = mInAudioDivide->GetData();

		const int32 NumSamples = AudioInput->Num();

		AudioDivideDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputAudioDivide, NumSamples);
	}

	METASOUND_REGISTER_NODE(FAudioDivideNode)
}

#undef LOCTEXT_NAMESPACE
