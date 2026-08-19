using UnrealBuildTool;

public class AsphaltRacer : ModuleRules
{
    public AsphaltRacer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[]
        {
            ModuleDirectory + "/AI",
            ModuleDirectory + "/Core",
            ModuleDirectory + "/Effects",
            ModuleDirectory + "/Systems",
            ModuleDirectory + "/Tracks",
            ModuleDirectory + "/UI",
            ModuleDirectory + "/Vehicles"
        });

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",
            "UMG",
            "Slate",
            "SlateCore",
            "ChaosVehicles",
            "PhysicsCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
