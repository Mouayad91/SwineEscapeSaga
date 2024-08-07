using UnrealBuildTool;

public class SwineEscapeSaga : ModuleRules
{
    public SwineEscapeSaga(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "Paper2D"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
