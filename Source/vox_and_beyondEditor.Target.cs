// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class vox_and_beyondEditorTarget : TargetRules
{
	public vox_and_beyondEditorTarget( TargetInfo Target) : base(Target)
	{
    bOverrideBuildEnvironment = true;
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bDebugBuildsActuallyUseDebugCRT = true; 
		ExtraModuleNames.AddRange( new string[] { "vox_and_beyond" } );
	}
}
