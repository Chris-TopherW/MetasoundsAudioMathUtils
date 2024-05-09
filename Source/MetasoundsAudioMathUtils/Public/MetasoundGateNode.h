// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FGateOperator
	//------------------------------------------------------------------------------------
	class FGateOperator : public TExecutableOperator<FGateOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FGateOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InGateToggle);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInGateToggle;

		DSPProcessing::FGate GateDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FGateNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FGateNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FGateNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FGateOperator>())
		{

		}
	};
}
