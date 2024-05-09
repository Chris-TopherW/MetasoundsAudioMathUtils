// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "AudioUtils.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

#include "CoreMinimal.h"
#include "MetasoundFacade.h"
#include "MetasoundVertex.h"

#pragma once

namespace Metasound
{
enum class EAudioComparisonType
{
	Equals,
	NotEquals,
	LessThan,
	GreaterThan,
	LessThanOrEquals,
	GreaterThanOrEquals
};

DECLARE_METASOUND_ENUM(EAudioComparisonType, EAudioComparisonType::Equals, METASOUNDSAUDIOMATHUTILS_API,
	FEnumAudioCompareType, FEnumAudioCompareTypeInfo, FEnumAudioCompareTypeReadRef, FEnumAudioCompareTypeWriteRef);

//------------------------------------------------------------------------------------
// FCompareOperator
//------------------------------------------------------------------------------------
class FCompareOperator : public TExecutableOperator<FCompareOperator>
{
public:
	static const FNodeClassMetadata& GetNodeInfo();
	static const FVertexInterface& GetVertexInterface();
	static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

	FCompareOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InCompareComparator, FEnumAudioCompareTypeReadRef& InComparisonTypeReadRef);

	virtual void BindInputs(FInputVertexInterfaceData& InOutVertexData) override;
	virtual void BindOutputs(FOutputVertexInterfaceData& InOutVertexData) override;

	void Execute();

private:
	FAudioBufferReadRef	 AudioInput;
	FAudioBufferWriteRef AudioOutput;
	FAudioBufferReadRef	 mInCompareComparator;
	FEnumAudioCompareTypeReadRef mComparisonType;
};

//------------------------------------------------------------------------------------
// FCompareNode
//------------------------------------------------------------------------------------
class METASOUNDSAUDIOMATHUTILS_API FCompareNode : public FNodeFacade
{
public:
	// Constructor used by the Metasound Frontend.
	FCompareNode(const FNodeInitData& InitData)
		: FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FCompareOperator>())
	{

	}
};
}
