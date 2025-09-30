// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundTimerNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_TimerNode"

namespace Metasound
{

	namespace TimerNode
	{
	METASOUND_PARAM(InputInTimerNode, "In", "Input trigger which outputs time since last trigger.");
	METASOUND_PARAM(OutputTimeOnTrigger, "Out", "The time between triggers.");
	} // namespace TimerNodeVertexNames

	FTimerOperator::FTimerOperator(const FOperatorSettings& InSettings, const FBuildOperatorParams& InParams, const FTriggerReadRef& InTriggerIn)
		: SampleRate(InSettings.GetSampleRate())
		, TriggerIn(InTriggerIn)
		, mTimeSeconds(TDataWriteReferenceFactory<FTime>::CreateExplicitArgs(InParams.OperatorSettings))
	{
	}

	void FTimerOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace TimerNode;
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InputInTimerNode), TriggerIn);
	}

	void FTimerOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace TimerNode;
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutputTimeOnTrigger), mTimeSeconds);
	}

	FDataReferenceCollection FTimerOperator::GetInputs() const
	{
		// This should never be called. Bind(...) is called instead. This method
		// exists as a stop-gap until the API can be deprecated and removed.
		checkNoEntry();
		return {};
	}

	FDataReferenceCollection FTimerOperator::GetOutputs() const
	{
		// This should never be called. Bind(...) is called instead. This method
		// exists as a stop-gap until the API can be deprecated and removed.
		checkNoEntry();
		return {};
	}

	void FTimerOperator::Execute()
	{


		TriggerIn->ExecuteBlock(
			[&](int32 StartFrame, int32 EndFrame)
			{
				mSampsSinceLastTrigger += EndFrame - StartFrame;
			},
			[this](int32 StartFrame, int32 EndFrame)
			{
				float NewTimeSeconds = ((float)mSampsSinceLastTrigger + (float)StartFrame) / (float)SampleRate;
				*mTimeSeconds = FTime(NewTimeSeconds);

				mSampsSinceLastTrigger = 0;
				mSampsSinceLastTrigger += EndFrame - StartFrame;
			}
		);
	}

	TUniquePtr<IOperator> FTimerOperator::CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors)
	{
		using namespace TimerNode;

		const FInputVertexInterfaceData& InputInterface = InParams.InputData;

		FTriggerReadRef TriggerIn = InputInterface.GetOrCreateDefaultDataReadReference<FTrigger>(METASOUND_GET_PARAM_NAME(InputInTimerNode), InParams.OperatorSettings);

		return MakeUnique<FTimerOperator>(InParams.OperatorSettings, InParams, TriggerIn);
	}

	const FVertexInterface& FTimerOperator::GetVertexInterface()
	{
		using namespace TimerNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(InputInTimerNode))
			),
			FOutputVertexInterface(
				TOutputDataVertex<FTime>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutputTimeOnTrigger))
			)
		);

		return Interface;
	}

	const FNodeClassMetadata& FTimerOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("Timer"), TEXT("") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_TimerNodeDisplayName", "Timer");
				Info.Description = LOCTEXT("Metasound_TimerNodeDescription", "Outputs time between triggers (does not support multiple triggers in single buffer callback)");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_RTimerNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();
		return Info;
	}


	FNodeClassMetadata FTimerNode::CreateNodeClassMetadata()
	{
		return FTimerOperator::GetNodeInfo();
	}

	METASOUND_REGISTER_NODE(FTimerNode)
}

#undef LOCTEXT_NAMESPACE