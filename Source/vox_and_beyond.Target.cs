// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class vox_and_beyondTarget : TargetRules
{
  public vox_and_beyondTarget(TargetInfo Target) : base(Target)
  {
    bOverrideBuildEnvironment = true;
    Type = TargetType.Game;
    bDebugBuildsActuallyUseDebugCRT = true;
    DefaultBuildSettings = BuildSettingsVersion.V2;

    ExtraModuleNames.AddRange(new string[] { "vox_and_beyond" });
    CppStandard = CppStandardVersion.Cpp17;
  }
}
