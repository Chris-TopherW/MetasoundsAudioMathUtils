#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FRawFIROperator
	//------------------------------------------------------------------------------------
	class FRawFIROperator : public TExecutableOperator<FRawFIROperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FRawFIROperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InAmplitude);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FRawFIR RawFIRDSPProcessor;

		FFloatReadRef mCoefficient;
	};

	//------------------------------------------------------------------------------------
	// FRawFIRNode
	//------------------------------------------------------------------------------------
	class FRawFIRNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FRawFIRNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FRawFIROperator>())
		{

		}
	};
}
