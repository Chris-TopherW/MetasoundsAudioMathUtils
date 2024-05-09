// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FCosOperator
	//------------------------------------------------------------------------------------
	class FCosOperator : public TExecutableOperator<FCosOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FCosOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FCos CosDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FCosNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FCosNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FCosNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FCosOperator>())
		{

		}
	};
}
