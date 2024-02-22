#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FMinusOperator
	//------------------------------------------------------------------------------------
	class FMinusOperator : public TExecutableOperator<FMinusOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FMinusOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InMinusComparator);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInMinusComparator;

		DSPProcessing::FMinus MinusDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FMinusNode
	//------------------------------------------------------------------------------------
	class FMinusNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FMinusNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FMinusOperator>())
		{

		}
	};
}
