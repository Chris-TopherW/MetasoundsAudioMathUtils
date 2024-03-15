#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FEqualsOperator
	//------------------------------------------------------------------------------------
	class FEqualsOperator : public TExecutableOperator<FEqualsOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FEqualsOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InEqualsComparator);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInEqualsComparator;

		DSPProcessing::FEquals EqualsDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FEqualsNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FEqualsNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FEqualsNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FEqualsOperator>())
		{

		}
	};
}
