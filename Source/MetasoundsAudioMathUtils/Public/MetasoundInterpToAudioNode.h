#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
	//------------------------------------------------------------------------------------
	// InterpToAudioOperator
	//------------------------------------------------------------------------------------
	class FInterpToAudioOperator : public TExecutableOperator<FInterpToAudioOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FInterpToAudioOperator(const FOperatorSettings& InSettings, const FFloatReadRef& targetValue, const FTimeReadRef& timeForInterpolation);

		virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
		virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

		void Execute();

	private:
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FInterpToAudio InterpToAudioDSPProcessor;

		FFloatReadRef mTargetValue;
		FTimeReadRef mTimeForInterpolation;
	};

	//------------------------------------------------------------------------------------
	// FInterpToAudioNode
	//------------------------------------------------------------------------------------
	class METASOUNDSAUDIOMATHUTILS_API FInterpToAudioNode : public FNodeFacade
	{
	public:
		// Constructor used by the Metasound Frontend.
		FInterpToAudioNode(const FNodeInitData& InitData)
			: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FInterpToAudioOperator>())
		{

		}
	};
}
