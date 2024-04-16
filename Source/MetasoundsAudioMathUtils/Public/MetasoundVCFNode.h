#pragma once

#include "CytomicSVF.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FVCFOperator
	//------------------------------------------------------------------------------------
	class FVCFOperator : public TExecutableOperator<FVCFOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FVCFOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InVCFCutoff, const FAudioBufferReadRef& InVCFQ);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInVCFCutoff;
		FAudioBufferReadRef	 mInVCFQ;

		DSPProcessing::CytomicSVF mCytomicSVF;

	};

	//------------------------------------------------------------------------------------
	// FVCFNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FVCFNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FVCFNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FVCFOperator>())
		{

		}
	};
}
