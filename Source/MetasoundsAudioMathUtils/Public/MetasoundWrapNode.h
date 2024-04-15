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
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

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
	};
}
