// Copyright (c) 2018 Jhonny Hueller
#pragma once

#include "CoreMinimal.h"
#include "AssetEditorToolkit.h"
#include "NotifyHook.h"
#include "GraphEditor.h"
#include "IDetailsView.h"
#include "XD_CampGraph.h"

/**
 *
 */
class FXD_CampGraph_EditorToolkit : public FAssetEditorToolkit, public FNotifyHook
{
public:
	FXD_CampGraph_EditorToolkit();
	~FXD_CampGraph_EditorToolkit();

	// Inherited via FAssetEditorToolkit
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void SaveAsset_Execute() override;

	virtual void InitGraphAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost, UXD_CampGraph* InGraph);
	virtual void BlueprintCompiled();
private:

	static const FName DetailsTabId;
	static const FName GraphTabId;

	UXD_CampGraph* GraphAsset;
	TSharedPtr<SGraphEditor> EdGraphEditor;
	TSharedPtr<IDetailsView> DetailsWidget;

	TSharedPtr<FUICommandList> GraphEditorCommands;

	FGraphPanelSelectionSet GetSelectedNodes();
	TSharedRef<SDockTab> HandleTabManagerSpawnTabDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> HandleTabManagerSpawnTabGraph(const FSpawnTabArgs& Args);
	void BindToolkitCommands();

	//Delegates
	void OnCommandSelectAllNodes();
	bool CanSelectAllNodes();

	void OnCommandCopy();
	bool CanCopyNodes();

	void OnCommandPaste();
	bool CanPasteNodes();

	void OnCommandCut();
	bool CanCutNodes();

	void OnCommandDuplicate();
	bool CanDuplicateNodes();

	void OnCommandDelete();
	bool CanDeleteNodes();

};
