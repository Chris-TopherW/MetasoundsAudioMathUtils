#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FOnePoleIIROperator
	//------------------------------------------------------------------------------------
	class FOnePoleIIROperator : public TExecutableOperator<FOnePoleIIROperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FOnePoleIIROperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InCoefficientA, const FAudioBufferReadRef& InCoefficientB);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferReadRef	 mCoefficientA;
		FAudioBufferReadRef	 mCoefficientB;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FOnePoleIIR OnePoleIIRDSPProcessor;
	};

	//------------------------------------------------------------------------------------
	// FOnePoleIIRNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FOnePoleIIRNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FOnePoleIIRNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FOnePoleIIROperator>())
		{

		}
	};
}
