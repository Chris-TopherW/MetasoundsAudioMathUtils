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
#include "MetasoundTrigger.h"
#include "MetasoundAudioBuffer.h"
#include "MetasoundParamHelper.h"


namespace Metasound
{

	//------------------------------------------------------------------------------------
	// FClickOperator
	//------------------------------------------------------------------------------------
	class FClickOperator : public TExecutableOperator<FClickOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors);

		FClickOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTriggerIn);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		float SampleRate;
		FTriggerReadRef TriggerIn;
		FAudioBufferWriteRef AudioOutput;
	};

	//------------------------------------------------------------------------------------
	// FClickNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FClickNode : public FNodeFacade
	{
	public:
		FClickNode(const FNodeInitData& InInitData)
			: FNodeFacade(InInitData.InstanceName, InInitData.InstanceID, TFacadeOperatorClass<FClickOperator>())
		{
		}

		FClickNode(FNodeData InNodeData, TSharedRef<const FNodeClassMetadata> InClassMetadata)
			: FNodeFacade(InNodeData, InClassMetadata, TFacadeOperatorClass<FClickOperator>())
		{
		}

		static FNodeClassMetadata CreateNodeClassMetadata();
	};

} // namespace Metasound