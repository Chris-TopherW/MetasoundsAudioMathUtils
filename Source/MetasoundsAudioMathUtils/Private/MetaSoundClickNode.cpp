// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundClickNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_ClickNode"

namespace Metasound
{

	namespace ClickNode
	{
		// Input params
		METASOUND_PARAM(InputInClickNode, "In", "Trigger the 'click' signal");

		// Output params
		METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.");
	} // namespace ClickNodeVertexNames

	FClickOperator::FClickOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTriggerIn)
		: SampleRate(InSettings.GetSampleRate())
		, TriggerIn(InTriggerIn)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
	{
	}

	void FClickOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace ClickNode;
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InputInClickNode), TriggerIn);
	}

	void FClickOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace ClickNode;
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	void FClickOperator::Execute()
	{
		float* OutputAudio = AudioOutput->GetData();
		const int32 NumSamples = AudioOutput->Num();

		for (int samp = 0; samp < NumSamples; ++samp)
		{
			OutputAudio[samp] = 0.0f;
		}

		TriggerIn->ExecuteBlock(
			[&](int32 StartFrame, int32 EndFrame)
			{
			},
			[this, OutputAudio](int32 StartFrame, int32 EndFrame)
			{
				OutputAudio[StartFrame] = 1.0f;
			}
		);
	}

	TUniquePtr<IOperator> FClickOperator::CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors)
	{
		using namespace ClickNode;

		const FInputVertexInterfaceData& InputInterface = InParams.InputData;

		FTriggerReadRef TriggerIn = InputInterface.GetOrCreateDefaultDataReadReference<FTrigger>(METASOUND_GET_PARAM_NAME(InputInClickNode), InParams.OperatorSettings);

		return MakeUnique<FClickOperator>(InParams.OperatorSettings, TriggerIn);
	}

	const FVertexInterface& FClickOperator::GetVertexInterface()
	{
		using namespace ClickNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(InputInClickNode))
			),
			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	const FNodeClassMetadata& FClickOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Click"), TEXT("") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_ClickNodeDisplayName", "Click");
				Info.Description = LOCTEXT("Metasound_ClickNodeDescription", "Outputs a variety of 'click' signals for use in signal processing)");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_RClickNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();
		return Info;
	}


	FNodeClassMetadata FClickNode::CreateNodeClassMetadata()
	{
		return FClickOperator::GetNodeInfo();
	}

	METASOUND_REGISTER_NODE(FClickNode)
}

#undef LOCTEXT_NAMESPACE