#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FGreaterThanOperator
	//------------------------------------------------------------------------------------
	class FGreaterThanOperator : public TExecutableOperator<FGreaterThanOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FGreaterThanOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InGreaterThanComparator);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInGreaterThanComparator;

		DSPProcessing::FGreaterThan GreaterThanDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FGreaterThanNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FGreaterThanNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FGreaterThanNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FGreaterThanOperator>())
		{

		}
	};
}
