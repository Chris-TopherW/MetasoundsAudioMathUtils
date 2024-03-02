#include "MetasoundSampholdNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_SampholdNode"

namespace Metasound
{
	namespace SampholdNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameSampholdPhasor, "Samphold Phasor", "The samphold object samples its left input whenever its right input decreases in value - as a phasor does each period, so it was designed to be triggered by a phasor. Both inputs are audio signals.")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FSampholdOperator
	//------------------------------------------------------------------------------------
	FSampholdOperator::FSampholdOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InSampholdPhasor)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInSampholdPhasor(InSampholdPhasor)
	{

	}

	const FNodeClassMetadata& FSampholdOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Samphold (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_SampholdDisplayName", "Samphold (Audio)");
				Info.Description = LOCTEXT("Metasound_SampholdNodeDescription", "Passes input 1 through if input 2 does not round to 0, does not if input 2 rounds to 0");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_SampholdNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FSampholdOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace SampholdNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameSampholdPhasor), mInSampholdPhasor);
	}

	void FSampholdOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace SampholdNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FSampholdOperator::GetVertexInterface()
	{
		using namespace SampholdNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameSampholdPhasor))
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FSampholdOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace SampholdNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InSampholdPhasor = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameSampholdPhasor), InParams.OperatorSettings);


		return MakeUnique<FSampholdOperator>(InParams.OperatorSettings, AudioIn, InSampholdPhasor);
	}

	void FSampholdOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputSampholdPhasor = mInSampholdPhasor->GetData();

		const int32 NumSamples = AudioInput->Num();

		SampholdDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, InputSampholdPhasor, NumSamples);
	}

	METASOUND_REGISTER_NODE(FSampholdNode)
}

#undef LOCTEXT_NAMESPACE
