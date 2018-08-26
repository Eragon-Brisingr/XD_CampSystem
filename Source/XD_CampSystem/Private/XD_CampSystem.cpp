// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "XD_CampSystem.h"
#if WITH_EDITOR
#include <ISettingsModule.h>
#include <ISettingsSection.h>
#include "XD_CampSystemSetting.h"
#endif

#define LOCTEXT_NAMESPACE "FXD_CampSystemModule"

void FXD_CampSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_EDITOR
	// register settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "XD_CampSystemSettings",
			LOCTEXT("XD_CampSystemSettings", "XD_CampSystemSettings"),
			LOCTEXT("XD_CampSystemSettingsDescription", "Configure the XD_CampSystemSettings plug-in."),
			GetMutableDefault<UXD_CampSystemSetting>()
		);
	}
#endif //WITH_EDITOR
}

void FXD_CampSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_CampSystemModule, XD_CampSystem)