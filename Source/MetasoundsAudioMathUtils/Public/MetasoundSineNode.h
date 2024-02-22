#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FSineOperator
	//------------------------------------------------------------------------------------
	class FSineOperator : public TExecutableOperator<FSineOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FSineOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FSine SineDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FSineNode
	//------------------------------------------------------------------------------------
	class FSineNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FSineNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FSineOperator>())
		{

		}
	};
}
