// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FAudioDivideOperator
	//------------------------------------------------------------------------------------
	class FAudioDivideOperator : public TExecutableOperator<FAudioDivideOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FAudioDivideOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InAudioDivide);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInAudioDivide;

		DSPProcessing::FAudioDivide AudioDivideDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FAudioDivideNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FAudioDivideNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FAudioDivideNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FAudioDivideOperator>())
		{

		}
	};
}

