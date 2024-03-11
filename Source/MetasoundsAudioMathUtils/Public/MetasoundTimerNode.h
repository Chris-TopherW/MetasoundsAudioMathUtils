
#pragma once

#include "CoreMinimal.h"

#include "MetasoundBuilderInterface.h"
#include "MetasoundDataReferenceCollection.h"
#include "MetasoundExecutableOperator.h"
#include "MetasoundFacade.h"
#include "MetasoundNode.h"
#include "MetasoundNodeInterface.h"
#include "MetasoundNodeRegistrationMacro.h"
#include "MetasoundOperatorInterface.h"
#include "MetasoundPrimitives.h"
#include "MetasoundParamHelper.h"


namespace Metasound
{

class FTimerNodeOperator : public TExecutableOperator<FTimerNodeOperator>
{
public:
	static const FNodeClassMetadata& GetNodeInfo();
	static const FVertexInterface& GetVertexInterface();
	static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

	FTimerNodeOperator(const FOperatorSettings& InSettings, const FCreateOperatorParams& InParams, const FTriggerReadRef& InTriggerIn);

	virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
	virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;
	virtual FDataReferenceCollection GetInputs() const override;
	virtual FDataReferenceCollection GetOutputs() const override;

	void Execute();
	void Reset(const IOperator::FResetParams& InParams);

private:
	float SampleRate;
	FTriggerReadRef TriggerIn;
	FTriggerWriteRef TriggerOut;
	FTimeWriteRef mTimeSeconds;
	long mSampsSinceLastTrigger;
};

class FTimerNode : public FNodeFacade
{
public:
	FTimerNode(const FNodeInitData& InInitData)
		: FNodeFacade(InInitData.InstanceName, InInitData.InstanceID, TFacadeOperatorClass<FTimerNodeOperator>())
	{
	}
};

} // namespace Metasound