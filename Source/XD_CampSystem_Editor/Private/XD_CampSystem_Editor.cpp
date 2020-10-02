// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "XD_CampSystem_Editor.h"
#include <PropertyEditorModule.h>
#include "XD_CampConfig_Customization.h"
#include "GraphEditor/Utility/XD_CampEditor_ClassHelper.h"
#include "XD_CampInfo.h"
#include <AssetToolsModule.h>
#include "GraphEditor/AssetTypeActions/XD_CampGraph_AssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FXD_CampSystemModule_Editor"

void FXD_CampSystem_EditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
		PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("XD_CampConfig"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FXD_CampConfig_Customization::MakeInstance));
	}

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	{
		XD_CampGraph_AssetTypeActions = MakeShareable(new FXD_CampGraph_AssetTypeActions());
		AssetTools.RegisterAssetTypeActions(XD_CampGraph_AssetTypeActions.ToSharedRef());
	}
}

void FXD_CampSystem_EditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FPropertyEditorModule* PropertyModulePtr = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = *PropertyModulePtr;
		PropertyModule.UnregisterCustomPropertyTypeLayout(TEXT("XD_CampConfig"));
	}
	
	if (FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools"))
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();
		AssetTools.UnregisterAssetTypeActions(XD_CampGraph_AssetTypeActions.ToSharedRef());
	}
}

void FXD_CampSystem_EditorModule::StartNodeHelper()
{
	if (!NodeHelper.IsValid())
	{
		NodeHelper = MakeShareable(new FXD_CampEditor_ClassHelper(UXD_CampInfo::StaticClass()));
		FXD_CampEditor_ClassHelper::AddObservedBlueprintClasses(UXD_CampInfo::StaticClass());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_CampSystem_EditorModule, XD_CampSystem_Editor)