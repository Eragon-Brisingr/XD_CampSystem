// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampGraph_AssetTypeActions.h"
#include "XD_CampGraph.h"
#include "XD_CampGraph_EditorToolkit.h"
#include "ModuleManager.h"
#include "XD_CampSystem_Editor.h"

#define LOCTEXT_NAMESPACE "XD_CampGraph_AssetTypeActions"

FXD_CampGraph_AssetTypeActions::FXD_CampGraph_AssetTypeActions()
{
}

FText FXD_CampGraph_AssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "GraphTypeActions", "阵营图表");
}

UClass * FXD_CampGraph_AssetTypeActions::GetSupportedClass() const
{
	return UXD_CampGraph::StaticClass();
}

FColor FXD_CampGraph_AssetTypeActions::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FXD_CampGraph_AssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

bool FXD_CampGraph_AssetTypeActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

void FXD_CampGraph_AssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	FXD_CampSystem_EditorModule& Helper = FModuleManager::LoadModuleChecked<FXD_CampSystem_EditorModule>("XD_CampSystem_Editor");
	Helper.StartNodeHelper();

	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto Object = InObjects.CreateConstIterator(); Object; Object++)
	{
		auto Graph = Cast<UXD_CampGraph>(*Object);
		if (Graph != nullptr)
		{
			TSharedRef<FXD_CampGraph_EditorToolkit>EditorToolkit = MakeShareable(new FXD_CampGraph_EditorToolkit());
			EditorToolkit->InitGraphAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

#undef LOCTEXT_NAMESPACE