using UnrealBuildTool;
using System.Collections.Generic;

public class AsphaltRacerTarget : TargetRules
{
    public AsphaltRacerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.Add("AsphaltRacer");
    }
}
