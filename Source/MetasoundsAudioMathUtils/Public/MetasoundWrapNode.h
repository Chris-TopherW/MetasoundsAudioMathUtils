// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FWrapOperator
	//------------------------------------------------------------------------------------
	class FWrapOperator : public TExecutableOperator<FWrapOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors);

		FWrapOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FWrap WrapDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FWrapNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FWrapNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FWrapNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FWrapOperator>())
		{

		}

		FWrapNode(FNodeData InNodeData, TSharedRef<const FNodeClassMetadata> InClassMetadata)
			: FNodeFacade(InNodeData, InClassMetadata, TFacadeOperatorClass<FWrapOperator>())
		{

		}

		static FNodeClassMetadata CreateNodeClassMetadata();
	};
}
