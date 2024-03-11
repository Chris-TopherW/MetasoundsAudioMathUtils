#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

#pragma once

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
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FRZeroOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InAmplitude);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FRZero RZeroDSPProcessor;

		FFloatReadRef mCoefficient;
	};

	//------------------------------------------------------------------------------------
	// FRZeroNode
	//------------------------------------------------------------------------------------
	class FRZeroNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FRZeroNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FRZeroOperator>())
		{

		}
	};
}
