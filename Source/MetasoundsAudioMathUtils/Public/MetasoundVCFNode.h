// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "CytomicSVF.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"
#include "MetasoundFacade.h"
#include "MetasoundVertex.h"

namespace Metasound
{
	enum class EVCFFilterType
	{
		LowPass,
		BandPass,
		HighPass
	};

	DECLARE_METASOUND_ENUM(EVCFFilterType, EVCFFilterType::LowPass, METASOUNDSAUDIOMATHUTILS_API,
		FEnumVCFFilterType, FEnumVCFFilterTypeInfo, FEnumVCFFilterTypeReadRef, FEnumVCFFilterTypeWriteRef);

	//------------------------------------------------------------------------------------
	// FVCFOperator
	//------------------------------------------------------------------------------------
	class FVCFOperator : public TExecutableOperator<FVCFOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FVCFOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InVCFCutoff, const FAudioBufferReadRef& InVCFQ, FEnumVCFFilterTypeReadRef& filterType);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

		void Init();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInVCFCutoff;
		FAudioBufferReadRef	 mInVCFQ;

		DSPProcessing::CytomicSVF mCytomicSVF;

		FEnumVCFFilterTypeReadRef mFilterType;

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
