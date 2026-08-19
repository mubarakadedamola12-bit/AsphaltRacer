using UnrealBuildTool;
using System.Collections.Generic;

public class AsphaltRacerEditorTarget : TargetRules
{
    public AsphaltRacerEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.Add("AsphaltRacer");
    }
}
