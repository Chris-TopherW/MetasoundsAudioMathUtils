// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FRZeroOperator
	//------------------------------------------------------------------------------------
	class FRZeroOperator : public TExecutableOperator<FRZeroOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors);

		FRZeroOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, FFloatReadRef& A);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FFloatReadRef A;
		float PriorSample;

		DSPProcessing::FRZero RZeroDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FRZeroNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FRZeroNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FRZeroNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FRZeroOperator>())
		{

		}

		FRZeroNode(FNodeData InNodeData, TSharedRef<const FNodeClassMetadata> InClassMetadata)
			: FNodeFacade(InNodeData, InClassMetadata, TFacadeOperatorClass<FRZeroOperator>())
		{

		}

		static FNodeClassMetadata CreateNodeClassMetadata();
	};
}
