// Copyright Christopher Wratt 2024.
// All code under MIT license: see https://mit-license.org/

using UnrealBuildTool;

public class MetasoundsAudioMathUtils : ModuleRules
{
	public MetasoundsAudioMathUtils(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);	
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
				// ... add public dependencies that you statically link with here ...
				"Core",
                "MetasoundFrontend",
                "MetasoundGraphCore",
                "AudioExtensions",
                "Serialization",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
				// ... add private dependencies that you statically link with here ...	
				"CoreUObject",
                "Engine",
                "SignalProcessing",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
