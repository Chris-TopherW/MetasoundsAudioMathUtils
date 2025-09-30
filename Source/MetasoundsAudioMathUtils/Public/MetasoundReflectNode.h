// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FReflectOperator
	//------------------------------------------------------------------------------------
	class FReflectOperator : public TExecutableOperator<FReflectOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FBuildOperatorParams& InParams, FBuildResults& OutErrors);

		FReflectOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FReflect ReflectDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FReflectNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FReflectNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FReflectNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FReflectOperator>())
		{

		}

		FReflectNode(FNodeData InNodeData, TSharedRef<const FNodeClassMetadata> InClassMetadata)
			: FNodeFacade(InNodeData, InClassMetadata, TFacadeOperatorClass<FReflectOperator>())
		{

		}

		static FNodeClassMetadata CreateNodeClassMetadata();
	};
}
