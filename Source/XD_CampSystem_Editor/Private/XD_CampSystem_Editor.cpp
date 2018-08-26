// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "XD_CampSystem_Editor.h"
#include <PropertyEditorModule.h>
#include "XD_PropertyCustomizationEx.h"
#include "XD_CampConfig_Customization.h"

#define LOCTEXT_NAMESPACE "FXD_CampSystemModule_Editor"

void FXD_CampSystem_EditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		RegisterCustomProperty(struct FXD_CampConfig, FXD_CampConfig_Customization);
	}
}

void FXD_CampSystem_EditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_CampSystem_EditorModule, XD_CampSystem_Editor)