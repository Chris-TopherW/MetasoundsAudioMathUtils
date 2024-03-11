// Fill out your copyright notice in the Description page of Project Settings.


#include "MetasoundTimerNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_TimerNode"

namespace Metasound
{

namespace TimerNode
{
METASOUND_PARAM(InputInTimerNode, "In", "Input trigger which outputs time since last trigger.");
METASOUND_PARAM(OutputTimeOnTrigger, "Out", "The time between triggers.");
METASOUND_PARAM(OutputOnTrigger, "Out", "Trigger out on trigger in.");
} // namespace TimerNodeVertexNames

FTimerNodeOperator::FTimerNodeOperator(const FOperatorSettings& InSettings, const FCreateOperatorParams& InParams, const FTriggerReadRef& InTriggerIn)
	: SampleRate(InSettings.GetSampleRate())
	, TriggerIn(InTriggerIn)
	, TriggerOut(FTriggerWriteRef::CreateNew(InSettings))
	, mTimeSeconds(TDataWriteReferenceFactory<FTime>::CreateExplicitArgs(InParams.OperatorSettings))
{
}

void FTimerNodeOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
{
	using namespace TimerNode;
	InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InputInTimerNode), TriggerIn);
}

void FTimerNodeOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
{
	using namespace TimerNode;
	//InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutputOnTrigger), TriggerOut);
	InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutputTimeOnTrigger), mTimeSeconds);
}

FDataReferenceCollection FTimerNodeOperator::GetInputs() const
{
	// This should never be called. Bind(...) is called instead. This method
	// exists as a stop-gap until the API can be deprecated and removed.
	checkNoEntry();
	return {};
}

FDataReferenceCollection FTimerNodeOperator::GetOutputs() const
{
	// This should never be called. Bind(...) is called instead. This method
	// exists as a stop-gap until the API can be deprecated and removed.
	checkNoEntry();
	return {};
}

void FTimerNodeOperator::Execute()
{
	// Advance internal counter to get rid of old triggers.
	TriggerOut->AdvanceBlock();


	TriggerIn->ExecuteBlock(
		[&](int32 StartFrame, int32 EndFrame)
		{
			mSampsSinceLastTrigger += EndFrame - StartFrame;
		},
		[this](int32 StartFrame, int32 EndFrame)
		{
			TriggerOut->TriggerFrame(StartFrame);
			float NewTimeSeconds = ((float)mSampsSinceLastTrigger + (float)StartFrame) / (float)SampleRate;
			*mTimeSeconds = FTime(NewTimeSeconds);

			mSampsSinceLastTrigger = 0;
			mSampsSinceLastTrigger += EndFrame - StartFrame;
		}
	);
}

void FTimerNodeOperator::Reset(const IOperator::FResetParams& InParams)
{
	TriggerOut->Reset();
}

TUniquePtr<IOperator> FTimerNodeOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
{
	using namespace TimerNode;

	const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

	FTriggerReadRef TriggerIn = InParams.InputDataReferences.GetDataReadReferenceOrConstruct<FTrigger>(METASOUND_GET_PARAM_NAME(InputInTimerNode), InParams.OperatorSettings);

	return MakeUnique<FTimerNodeOperator>(InParams.OperatorSettings, InParams, TriggerIn);
}

const FVertexInterface& FTimerNodeOperator::GetVertexInterface()
{
	using namespace TimerNode;

	static const FVertexInterface Interface(
		FInputVertexInterface(
			TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(InputInTimerNode))
		),
		FOutputVertexInterface(
			//TOutputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutputOnTrigger)),
			TOutputDataVertex<FTime>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutputTimeOnTrigger))
		)
	);

	return Interface;
}

const FNodeClassMetadata& FTimerNodeOperator::GetNodeInfo()
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

METASOUND_REGISTER_NODE(FTimerNode)
}

#undef LOCTEXT_NAMESPACE