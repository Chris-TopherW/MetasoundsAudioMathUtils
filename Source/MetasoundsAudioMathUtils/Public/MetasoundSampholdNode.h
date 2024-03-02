#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FSampholdOperator
	//------------------------------------------------------------------------------------
	class FSampholdOperator : public TExecutableOperator<FSampholdOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FSampholdOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InSampholdPhasor);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInSampholdPhasor;

		DSPProcessing::FSamphold SampholdDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FSampholdNode
	//------------------------------------------------------------------------------------
	class FSampholdNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FSampholdNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FSampholdOperator>())
		{

		}
	};
}
