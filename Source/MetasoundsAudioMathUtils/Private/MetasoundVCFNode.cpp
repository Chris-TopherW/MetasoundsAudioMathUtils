// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#include "MetasoundVCFNode.h"

#define LOCTEXT_NAMESPACE "MetasoundNodeTemplate_VCFNode"

namespace Metasound
{
	DEFINE_METASOUND_ENUM_BEGIN(EVCFFilterType, FEnumVCFFilterType, "VCFFilterType")
		DEFINE_METASOUND_ENUM_ENTRY(EVCFFilterType::LowPass, "LowPassDescription", "LowPass", "LowPassDescriptionTT", ""),
		DEFINE_METASOUND_ENUM_ENTRY(EVCFFilterType::BandPass, "BandPassDescriptioin", "BandPass", "BandPassDescriptioinTT", ""),
		//DEFINE_METASOUND_ENUM_ENTRY(EVCFFilterType::HighPass, "HighPassDescription", "HighPass", "HighPassDescriptionTT", ""),
		DEFINE_METASOUND_ENUM_END()

	namespace VCFNode
	{
		// Input params
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
			METASOUND_PARAM(InParamNameVCFCutoff, "Cutoff Hz", "Cutoff freq in Hz")
			METASOUND_PARAM(InParamNameVCFQ, "Q", "Q of 0.707 means no resonant peaking")
			METASOUND_PARAM(InParamNameVCOFilterType, "FilterType", "The type of filter to apply")

			// Output params
			METASOUND_PARAM(OutParamNameAudio, "Out", "Audio output.")
	}

	//------------------------------------------------------------------------------------
	// FVCFOperator
	//------------------------------------------------------------------------------------
	FVCFOperator::FVCFOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InVCFCutoff, const FAudioBufferReadRef& InVCFQ, FEnumVCFFilterTypeReadRef& filterType)
		: AudioInput(InAudioInput)
		, AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
		, mInVCFCutoff(InVCFCutoff)
		, mInVCFQ(InVCFQ)
		, mFilterType(filterType)
	{
		Init();
	}

	const FNodeClassMetadata& FVCFOperator::GetNodeInfo()
	{
		auto InitNodeInfo = []() -> FNodeClassMetadata
			{
				FNodeClassMetadata Info;

				Info.ClassName = { TEXT("UE"), TEXT("VCF (Audio)"), TEXT("Audio") };
				Info.MajorVersion = 1;
				Info.MinorVersion = 0;
				Info.DisplayName = LOCTEXT("Metasound_VCFDisplayName", "VCF (Audio)");
				Info.Description = LOCTEXT("Metasound_VCFNodeDescription", "Filter that can modulate up to nyquist: 2 pole, 12dB / 8ve. From http://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf, not high pass is currently bugged!");
				Info.Author = "Chris Wratt";
				Info.PromptIfMissing = PluginNodeMissingPrompt;
				Info.DefaultInterface = GetVertexInterface();
				Info.CategoryHierarchy = { LOCTEXT("Metasound_VCFNodeCategory", "Utils") };

				return Info;
			};

		static const FNodeClassMetadata Info = InitNodeInfo();

		return Info;
	}

	void FVCFOperator::BindInputs(FInputVertexInterfaceData& InOutVertexData)
	{
		using namespace VCFNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameVCFCutoff), mInVCFCutoff);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameVCFQ), mInVCFQ);
		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(InParamNameVCOFilterType), mFilterType);
	}

	void FVCFOperator::BindOutputs(FOutputVertexInterfaceData& InOutVertexData)
	{
		using namespace VCFNode;

		InOutVertexData.BindReadVertex(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);
	}

	const FVertexInterface& FVCFOperator::GetVertexInterface()
	{
		using namespace VCFNode;

		static const FVertexInterface Interface(
			FInputVertexInterface(
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameVCFCutoff)),
				TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameVCFQ)),
				TInputDataVertex<FEnumVCFFilterType>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameVCOFilterType), (int32)EVCFFilterType::LowPass)
			),

			FOutputVertexInterface(
				TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
			)
		);

		return Interface;
	}

	TUniquePtr<IOperator> FVCFOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
	{
		using namespace VCFNode;

		const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
		const FInputVertexInterface& InputInterface = GetVertexInterface().GetInputInterface();

		FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
		FAudioBufferReadRef InVCFCutoff = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameVCFCutoff), InParams.OperatorSettings);
		FAudioBufferReadRef InVCFQ = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameVCFQ), InParams.OperatorSettings);
		FEnumVCFFilterTypeReadRef InFilterType = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FEnumVCFFilterType>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameVCOFilterType), InParams.OperatorSettings);


		return MakeUnique<FVCFOperator>(InParams.OperatorSettings, AudioIn, InVCFCutoff, InVCFQ, InFilterType);
	}

	void FVCFOperator::Execute()
	{
		const float* InputAudio = AudioInput->GetData();
		float* OutputAudio = AudioOutput->GetData();
		const float* InputVCFCutoff = mInVCFCutoff->GetData();
		const float* InputVCFQ = mInVCFQ->GetData();

		const int32 NumSamples = AudioInput->Num();

		mCytomicSVF.ProcessAudioBuffer(InputAudio, OutputAudio, InputVCFCutoff, InputVCFQ, NumSamples);
	}

	void FVCFOperator::Init()
	{
		// need to put a proper watcher on this rather than just calling all the time.
		switch (*mFilterType)
		{
		case EVCFFilterType::LowPass:
			mCytomicSVF.SetLPF();
			break;

		case EVCFFilterType::BandPass:
			mCytomicSVF.setBP();
			break;

		//case EVCFFilterType::HighPass:
		//	mCytomicSVF.setHPF();
		//	break;
		}
	}

	METASOUND_REGISTER_NODE(FVCFNode)
}

#undef LOCTEXT_NAMESPACE
