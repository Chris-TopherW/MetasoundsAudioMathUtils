// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundRZeroNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_RZeroNode"

namespace Metasound
{
	namespace RZeroNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
		METASOUND_PARAM(InParamNameA, "A", "Coefficient for prior sample. 1.0 for differentiation.")
		// Output params
		METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FRZeroOperator
	//------------------------------------------------------------------------------------
	FRZeroOperator::FRZeroOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, FFloatReadRef& InA)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, A{MoveTemp(InA)}
		, PriorSample(NAN)
	{

	}

	const FNodeClassMetadata& FRZeroOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("RZero (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_RZeroDisplayName", "RZero (Audio)");
				Info.Description = LOCTEXT("Metasound_RZeroNodeDescription", "Applies PureData RZero filter y[n] = x[n] - a[n] * x[n-1].");
				Info.Author = "geekrelief";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_RZeroNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FRZeroOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace RZeroNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameA), A);
	}

	void FRZeroOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace RZeroNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FRZeroOperator::GetVertexInterface()
	{
		using namespace RZeroNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameA), 1.f)
				),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FRZeroOperator::CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors)
	{
		using namespace RZeroNode;

		const FInputVertexInterfaceData& InputInterface = InParams.InputData;

		FAudioBufferReadRef AudioIn = InputInterface.GetOrCreateDefaultDataReadReference<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FFloatReadRef AIn = InputInterface.GetOrCreateDefaultDataReadReference<float>(METASOUND_GET_PARAM_NAME(InParamNameA), InParams.OperatorSettings);

		return MakeUnique<FRZeroOperator>(InParams.OperatorSettings, AudioIn, AIn);
	}

	void FRZeroOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();

		const int32 NumSamples = AudioInput->Num();

		PriorSample = RZeroDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, *A, PriorSample, NumSamples);
	}


	FNodeClassMetadata FRZeroNode::CreateNodeClassMetadata()
	{
		return FRZeroOperator::GetNodeInfo();
	}

	METASOUND_REGISTER_NODE(FRZeroNode)
}

#undef LOCTEXT_NAMESPACE
