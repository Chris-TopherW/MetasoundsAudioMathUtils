#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FNotEqualsOperator
	//------------------------------------------------------------------------------------
	class FNotEqualsOperator : public TExecutableOperator<FNotEqualsOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FNotEqualsOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InNotEqualsComparator);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInNotEqualsComparator;

		DSPProcessing::FNotEquals NotEqualsDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FNotEqualsNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FNotEqualsNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FNotEqualsNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FNotEqualsOperator>())
		{

		}
	};
}
