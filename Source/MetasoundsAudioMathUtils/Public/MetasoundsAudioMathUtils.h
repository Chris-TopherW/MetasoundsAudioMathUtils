// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class METASOUNDSAUDIOMATHUTILS_API FMetasoundsAudioMathUtilsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
