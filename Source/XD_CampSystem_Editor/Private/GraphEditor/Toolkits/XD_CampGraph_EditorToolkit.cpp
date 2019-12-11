// Copyright (c) 2018 Jhonny Hueller
#include "GraphEditor/Toolkits/XD_CampGraph_EditorToolkit.h"
#include "GraphEditor/Utility/XD_CampSystemEditor_Log.h"
#include <PropertyEditorModule.h>
#include <Modules/ModuleManager.h>
#include <Slate.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <GraphEditorActions.h>
#include "GraphEditor/EditorGraph/XD_CampEditorGraph.h"
#include "GraphEditor/EditorGraph/XD_CampEditor_GraphSchema.h"
#include <EdGraphUtilities.h>
#include <HAL/PlatformApplicationMisc.h>
#include "GraphEditor/EditorGraph/EditorNodes/XD_CampGraph_EditorNode.h"
#include <Editor.h>
#include <ScopedTransaction.h>


#define LOCTEXT_NAMESPACE "XD_CampGraph_EditorToolkit"

const FName FXD_CampGraph_EditorToolkit::DetailsTabId(TEXT("XD_CampGraph_EditorToolkitDetailsTabId"));
const FName FXD_CampGraph_EditorToolkit::GraphTabId(TEXT("XD_CampGraph_EditorToolkitGraphTabId"));


FXD_CampGraph_EditorToolkit::FXD_CampGraph_EditorToolkit()
{
	GEditor->OnBlueprintCompiled().AddRaw(this,&FXD_CampGraph_EditorToolkit::BlueprintCompiled);
}

FXD_CampGraph_EditorToolkit::~FXD_CampGraph_EditorToolkit()
{
	GEditor->OnBlueprintCompiled().RemoveAll(this);
}

FGraphPanelSelectionSet FXD_CampGraph_EditorToolkit::GetSelectedNodes()
{
	return EdGraphEditor->GetSelectedNodes();
}

TSharedRef<SDockTab> FXD_CampGraph_EditorToolkit::HandleTabManagerSpawnTabDetails(const FSpawnTabArgs & Args)
{

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bCustomNameAreaLocation = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.NotifyHook = this;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	DetailsWidget = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsWidget->SetObject(GraphAsset);

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			DetailsWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FXD_CampGraph_EditorToolkit::HandleTabManagerSpawnTabGraph(const FSpawnTabArgs & Args)
{

	if (GraphAsset->EdGraph != nullptr)
	{
		EdGraphEditor = SNew(SGraphEditor)
			.AdditionalCommands(GraphEditorCommands)
			.GraphToEdit(GraphAsset->EdGraph);
	}
	else
	{
		CampSystemEditor_Error_Log("There is no editor graph inside the graph asset.");
	}
	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			EdGraphEditor.ToSharedRef()
		];
}

FLinearColor FXD_CampGraph_EditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Blue;
}

FName FXD_CampGraph_EditorToolkit::GetToolkitFName() const
{
	return FName("Graph Editor");
}

FText FXD_CampGraph_EditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Graph Editor");
}

FString FXD_CampGraph_EditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Graph").ToString();
}

void FXD_CampGraph_EditorToolkit::InitGraphAssetEditor(const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost, UXD_CampGraph * InGraph)
{
	GraphAsset = InGraph;
	if (GraphAsset->EdGraph == nullptr)
	{
		CampSystemEditor_Log("Creating a new graph.");
		GraphAsset->EdGraph = CastChecked<UXD_CampEditorGraph>(FBlueprintEditorUtils::CreateNewGraph(GraphAsset, NAME_None, UXD_CampEditorGraph::StaticClass(), UXD_CampEditor_GraphSchema::StaticClass()));
		GraphAsset->EdGraph->bAllowDeletion = false;

		//Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = GraphAsset->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*GraphAsset->EdGraph);
	}

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	BindToolkitCommands();

	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(TEXT("CampGraphEditorLayout"))
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
				->SetHideTabWell(true)
				->SetSizeCoefficient(0.2f)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.8f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(DetailsTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.15f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(GraphTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.85f)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(InMode, InToolkitHost, FName("GraphEditorIdentifier"), Layout, true, true, GraphAsset);
}

void FXD_CampGraph_EditorToolkit::BlueprintCompiled()
{
	UEdGraph* EdGraph = EdGraphEditor->GetCurrentGraph();
	if (UXD_CampEditorGraph* MyGraph = Cast<UXD_CampEditorGraph>(EdGraph))
		MyGraph->RefreshNodes();
}

void FXD_CampGraph_EditorToolkit::SaveAsset_Execute()
{
	if (GraphAsset && GraphAsset->EdGraph)
	{
		UXD_CampEditorGraph* EdGraph = Cast<UXD_CampEditorGraph>(GraphAsset->EdGraph);
		EdGraph->SaveGraph();
	}
	FAssetEditorToolkit::SaveAsset_Execute();
}

void FXD_CampGraph_EditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("XD_CampEditorGraphToolkitWorkspaceMenu", "Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &FXD_CampGraph_EditorToolkit::HandleTabManagerSpawnTabDetails))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef);
	InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateSP(this, &FXD_CampGraph_EditorToolkit::HandleTabManagerSpawnTabGraph))
		.SetDisplayName(LOCTEXT("GraphTab", "Graph Editor"))
		.SetGroup(WorkspaceMenuCategoryRef);

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FXD_CampGraph_EditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
	InTabManager->UnregisterTabSpawner(GraphTabId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commands and Bindings
void FXD_CampGraph_EditorToolkit::BindToolkitCommands()
{
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList());

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::OnCommandSelectAllNodes),
			FCanExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::CanSelectAllNodes)
		);

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Cut,
			FExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::OnCommandCut),
			FCanExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::CanCutNodes)
		);

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Copy,
			FExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::OnCommandCopy),
			FCanExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::CanCopyNodes)
		);

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Paste,
			FExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::OnCommandPaste),
			FCanExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::CanPasteNodes)
		);

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::OnCommandDuplicate),
			FCanExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::CanDuplicateNodes)
		);

		GraphEditorCommands->MapAction
		(
			FGenericCommands::Get().Delete,
			FExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::OnCommandDelete),
			FCanExecuteAction::CreateRaw(this, &FXD_CampGraph_EditorToolkit::CanDeleteNodes)
		);

	}
}

void FXD_CampGraph_EditorToolkit::OnCommandSelectAllNodes()
{
	if (EdGraphEditor.IsValid())
		EdGraphEditor->SelectAllNodes();
}

bool FXD_CampGraph_EditorToolkit::CanSelectAllNodes()
{
	return true;
}

void FXD_CampGraph_EditorToolkit::OnCommandCut()
{
	const FScopedTransaction Transaction(FGenericCommands::Get().Cut->GetDescription());

	OnCommandCopy();

	const FGraphPanelSelectionSet OldSelectedNodes = EdGraphEditor->GetSelectedNodes();
	EdGraphEditor->ClearSelectionSet();
	for (FGraphPanelSelectionSet::TConstIterator It(OldSelectedNodes); It; ++It)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*It);
		if (Node && Node->CanDuplicateNode())
		{
			EdGraphEditor->SetNodeSelection(Node, true);
		}
	}

	OnCommandDelete();

	EdGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator It(OldSelectedNodes); It; ++It)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*It);
		if (Node)
			EdGraphEditor->SetNodeSelection(Node, true);
	}
}

bool FXD_CampGraph_EditorToolkit::CanCutNodes()
{
	return true;
}

void FXD_CampGraph_EditorToolkit::OnCommandCopy()
{
	const FScopedTransaction Transaction(FGenericCommands::Get().Copy->GetDescription());

	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator it(SelectedNodes); it; ++it)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*it);
		if (Node)
			Node->PrepareForCopying();
		else
			it.RemoveCurrent();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	for (FGraphPanelSelectionSet::TIterator it(SelectedNodes); it; ++it)
	{
		UXD_CampGraph_EditorNode* Node = Cast<UXD_CampGraph_EditorNode>(*it);
		if (Node)
			Node->PostCopyNode();
	}
}

bool FXD_CampGraph_EditorToolkit::CanCopyNodes()
{
	return true;
}

void FXD_CampGraph_EditorToolkit::OnCommandPaste()
{
	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());

	const FVector2D PasteLocation = EdGraphEditor->GetPasteLocation();

	UEdGraph* EdGraph = EdGraphEditor->GetCurrentGraph();
	EdGraph->Modify();
	EdGraphEditor->ClearSelectionSet();

	FString ExportedText;
	FPlatformApplicationMisc::ClipboardPaste(ExportedText);
	TSet<UEdGraphNode*> ImportedNodes;
	FEdGraphUtilities::ImportNodesFromText(EdGraph, ExportedText, ImportedNodes);

	for (TSet<UEdGraphNode*>::TIterator It(ImportedNodes); It; ++It)
	{
		UXD_CampGraph_EditorNode* Node = Cast<UXD_CampGraph_EditorNode>(*It);
		GraphAsset->AddCamp(Node->CampInfo);
	}

	FVector2D AvgNodePosition(0.0f, 0.0f);

	for (TSet<UEdGraphNode*>::TIterator It(ImportedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	float InvNumNodes = 1.0f / float(ImportedNodes.Num());
	AvgNodePosition.X *= InvNumNodes;
	AvgNodePosition.Y *= InvNumNodes;

	for (TSet<UEdGraphNode*>::TIterator It(ImportedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		EdGraphEditor->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + PasteLocation.X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + PasteLocation.Y;

		Node->SnapToGrid(16);

		// Give new node a different Guid from the old one
		Node->CreateNewGuid();
	}

	EdGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}

}

bool FXD_CampGraph_EditorToolkit::CanPasteNodes()
{
	return false;
}

void FXD_CampGraph_EditorToolkit::OnCommandDuplicate()
{
	OnCommandCopy();
	OnCommandPaste();
}

bool FXD_CampGraph_EditorToolkit::CanDuplicateNodes()
{
	return false;
}

void FXD_CampGraph_EditorToolkit::OnCommandDelete()
{
	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	EdGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	EdGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator It(SelectedNodes); It; ++It)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*It))
		{
			Node->GetSchema()->BreakNodeLinks(*Node);
			Node->Modify();
			Node->DestroyNode();
		}
	}
}

bool FXD_CampGraph_EditorToolkit::CanDeleteNodes()
{
	return true;
}

// END Commands and binding
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE