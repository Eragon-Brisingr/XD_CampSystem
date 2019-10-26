// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

struct FXD_CampEditor_ClassHelper;
class FXD_CampGraph_AssetTypeActions;

class FXD_CampSystem_EditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void StartNodeHelper();
	TSharedPtr<struct FXD_CampEditor_ClassHelper> GetHelper() { return NodeHelper; }
private:
	TSharedPtr<struct FXD_CampEditor_ClassHelper> NodeHelper;
	TSharedPtr<FXD_CampGraph_AssetTypeActions> XD_CampGraph_AssetTypeActions;
};
