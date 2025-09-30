// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

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
#include "MetasoundTrigger.h"

namespace Metasound
{

	//------------------------------------------------------------------------------------
	// FTimerOperator
	//------------------------------------------------------------------------------------
	class FTimerOperator : public TExecutableOperator<FTimerOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors);

		FTimerOperator(const FOperatorSettings& InSettings, const FBuildOperatorParams& InParams, const FTriggerReadRef& InTriggerIn);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;
		virtual FDataReferenceCollection GetInputs() const override;
		virtual FDataReferenceCollection GetOutputs() const override;

		void Execute();

	private:
		float SampleRate;
		FTriggerReadRef TriggerIn;
		FTimeWriteRef mTimeSeconds;
		long mSampsSinceLastTrigger;
	};

	//------------------------------------------------------------------------------------
	// FTimerNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FTimerNode : public FNodeFacade
	{
	public:
		FTimerNode(const FNodeInitData& InInitData)
			: FNodeFacade(InInitData.InstanceName, InInitData.InstanceID, TFacadeOperatorClass<FTimerOperator>())
		{
		}

		FTimerNode(FNodeData InNodeData, TSharedRef<const FNodeClassMetadata> InClassMetadata)
			: FNodeFacade(InNodeData, InClassMetadata, TFacadeOperatorClass<FTimerOperator>())
		{

		}

		static FNodeClassMetadata CreateNodeClassMetadata();
	};

} // namespace Metasound