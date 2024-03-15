#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// FLessThanOperator
	//------------------------------------------------------------------------------------
	class FLessThanOperator : public TExecutableOperator<FLessThanOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FLessThanOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InLessThanComparator);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferReadRef	 AudioInput;
		FAudioBufferWriteRef AudioOutput;

		FAudioBufferReadRef	 mInLessThanComparator;

		DSPProcessing::FLessThan LessThanDSPProcessor;

	};

	//------------------------------------------------------------------------------------
	// FLessThanNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FLessThanNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FLessThanNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FLessThanOperator>())
		{

		}
	};
}
