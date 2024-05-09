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


namespace Metasound
{

	class FClickNodeOperator : public TExecutableOperator<FClickNodeOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FClickNodeOperator(const FOperatorSettings& InSettings, const FCreateOperatorParams& InParams, const FTriggerReadRef& InTriggerIn);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		float SampleRate;
		FTriggerReadRef TriggerIn;
		FAudioBufferWriteRef AudioOutput;
	};

	class METASOUNDSAUDIOMATHUTILS_API FClickNode : public FNodeFacade
	{
	public:
		FClickNode(const FNodeInitData& InInitData)
			: FNodeFacade(InInitData.InstanceName, InInitData.InstanceID, TFacadeOperatorClass<FClickNodeOperator>())
		{
		}
	};

} // namespace Metasound