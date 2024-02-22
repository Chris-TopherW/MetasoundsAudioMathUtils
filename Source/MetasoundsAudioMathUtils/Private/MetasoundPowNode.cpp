#include "MetasoundPowNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_PowNode"

namespace Metasound
{
	namespace PowNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNamePowerOf, "Pow", "The value to test the input against")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FPowOperator
	//------------------------------------------------------------------------------------
	FPowOperator::FPowOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InPowerOf)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInPowerOf(InPowerOf)
	{

	}

	const FNodeClassMetadata& FPowOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Pow (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_PowDisplayName", "Pow (Audio)");
				Info.Description = LOCTEXT("Metasound_PowNodeDescription", "Returns input 1 to the power of input 2");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_PowNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FPowOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace PowNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNamePowerOf), mInPowerOf);
	}

	void FPowOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace PowNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FPowOperator::GetVertexInterface()
	{
		using namespace PowNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNamePowerOf))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FPowOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace PowNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InPowerOf = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNamePowerOf), InParams.OperatorSettings);


		return MakeUnique<FPowOperator>(InParams.OperatorSettings, AudioIn, InPowerOf);
	}

	void FPowOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputPowerOf = mInPowerOf->GetData();

		const int32 NumSamples = AudioInput->Num();

		PowDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputPowerOf, NumSamples);
	}

	METASOUND_REGISTER_NODE(FPowNode)
}

#undef LOCTEXT_NAMESPACE
