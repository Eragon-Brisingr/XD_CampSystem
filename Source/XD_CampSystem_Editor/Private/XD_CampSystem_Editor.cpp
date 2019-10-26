﻿// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "XD_CampSystem_Editor.h"
#include <PropertyEditorModule.h>
#include "XD_PropertyCustomizationEx.h"
#include "XD_CampConfig_Customization.h"
#include "XD_CampEditor_ClassHelper.h"
#include "XD_CampNode.h"
#include "AssetToolsModule.h"
#include "XD_CampGraph_AssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FXD_CampSystemModule_Editor"

void FXD_CampSystem_EditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		RegisterCustomProperty(struct FXD_CampConfig, FXD_CampConfig_Customization);
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
	
}

void FXD_CampSystem_EditorModule::StartNodeHelper()
{
	if (!NodeHelper.IsValid())
	{
		NodeHelper = MakeShareable(new FXD_CampEditor_ClassHelper(UXD_CampNode::StaticClass()));
		FXD_CampEditor_ClassHelper::AddObservedBlueprintClasses(UXD_CampNode::StaticClass());
	}

	if (FAssetToolsModule* AssetToolsModule = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools"))
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();
		AssetTools.UnregisterAssetTypeActions(XD_CampGraph_AssetTypeActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_CampSystem_EditorModule, XD_CampSystem_Editor)