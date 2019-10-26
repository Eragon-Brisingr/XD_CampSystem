// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampGraph_EditorNode.h"
#include "XD_CampNode.h"
#include "XD_CampGraph.h"
#include "XD_CampEditorGraph.h"
#include "SXD_CampEditorNode.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "ModuleManager.h"
#include "XD_CampSystemEditor_Log.h"
#include "GenericCommands.h"
#include "GraphEditorActions.h"

#define LOCTEXT_NAMESPACE "XD_CampGraph_EditorNode"

UXD_CampGraph_EditorNode::UXD_CampGraph_EditorNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

const FName DefualtPinType("Default");
const FName InPinName("InPin");
const FName OutPinName("OutPin");

TSharedPtr<SGraphNode> UXD_CampGraph_EditorNode::CreateVisualWidget()
{
	SlateNode= SNew(SXD_CampEditorNode, this);
	return SlateNode;
}

TSharedPtr<SWidget> UXD_CampGraph_EditorNode::GetContentWidget()
{
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedPtr<IDetailsView> View = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	View->SetObject(AssetNode);
	return View;
}

void UXD_CampGraph_EditorNode::UpdateVisualNode()
{
	SlateNode->UpdateGraphNode();
}

void UXD_CampGraph_EditorNode::SaveNodesAsChildren(TArray<UEdGraphNode*>& Children)
{
    for (UEdGraphNode* Child : Children)
    {
        UXD_CampGraph_EditorNode* Node = Cast<UXD_CampGraph_EditorNode>(Child);
        if (Node)
            AssetNode->LinkArgumentNodeAsChild(Node->AssetNode);
    }
}

bool UXD_CampGraph_EditorNode::HasOutputPins()
{
	return AssetNode ? AssetNode->HasOutputPins() : true;
}

bool UXD_CampGraph_EditorNode::HasInputPins()
{
	return AssetNode ? AssetNode->HasInputPins() : true;
}

void UXD_CampGraph_EditorNode::AllocateDefaultPins()
{
	UEdGraphNode::AllocateDefaultPins();
	if (HasInputPins())
		CreatePin(EGPD_Input, DefualtPinType, InPinName);
	if (HasOutputPins())
		CreatePin(EGPD_Output, DefualtPinType, OutPinName);
}

FText UXD_CampGraph_EditorNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    switch (TitleType)
    {
    case ENodeTitleType::Type::MenuTitle:
        return AssetNode->GetNodeTitle();
    default:
        FText Title = GetEdNodeName();
        return Title.IsEmpty() ? AssetNode->GetNodeTitle() : Title;
    }
}

void UXD_CampGraph_EditorNode::PrepareForCopying()
{
	if (AssetNode)
		AssetNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
	UEdGraphNode::PrepareForCopying();
}

void UXD_CampGraph_EditorNode::DestroyNode()
{
	if (AssetNode)
	{
		AssetNode->GetGraph()->RemoveNode(AssetNode);
		AssetNode->ConditionalBeginDestroy();
		AssetNode = nullptr;
	}
	UEdGraphNode::DestroyNode();
}

void UXD_CampGraph_EditorNode::AutowireNewNode(UEdGraphPin * FromPin)
{
	if (FromPin)
	{
		UEdGraphNode::AutowireNewNode(FromPin);
		if (FromPin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			if (GetSchema()->TryCreateConnection(FromPin, FindPin(OutPinName)))
			{
				FromPin->GetOwningNode()->NodeConnectionListChanged();
			}
		}
		if (FromPin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			if (GetSchema()->TryCreateConnection(FromPin, FindPin(InPinName)))
			{
				FromPin->GetOwningNode()->NodeConnectionListChanged();
			}
		}
	}
}


void UXD_CampGraph_EditorNode::GetContextMenuActions(const FGraphNodeContextMenuBuilder& Context) const
{
	FMenuBuilder* MenuBuilder = Context.MenuBuilder;
	MenuBuilder->BeginSection(NAME_None, LOCTEXT("NodeActionsMenuHeader", "节点操作"));
	{
		MenuBuilder->AddMenuEntry(FGenericCommands::Get().Delete);
		MenuBuilder->AddMenuEntry(FGenericCommands::Get().Cut);
		MenuBuilder->AddMenuEntry(FGenericCommands::Get().Copy);
		MenuBuilder->AddMenuEntry(FGenericCommands::Get().Duplicate);

		MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}
	MenuBuilder->EndSection();
}

void UXD_CampGraph_EditorNode::SetAssetNode(UXD_CampNode * InNode)
{
	AssetNode = InNode;
}

UXD_CampNode* UXD_CampGraph_EditorNode::GetAssetNode()
{
	return AssetNode;
}

void UXD_CampGraph_EditorNode::PostCopyNode()
{
	if (AssetNode)
	{
		UEdGraph* EdGraph = GetGraph();
		UObject* ParentAsset = EdGraph ? EdGraph->GetOuter() : nullptr;
		AssetNode->Rename(nullptr, ParentAsset, REN_DontCreateRedirectors | REN_DoNotDirty);
		AssetNode->ClearFlags(RF_Transient);
	}
}

bool UXD_CampGraph_EditorNode::RenameUniqueNode(const FText & NewName)
{
	bool bRenamedNode = false;

	UXD_CampEditorGraph* EdGraph = Cast<UXD_CampEditorGraph>(GetGraph());

	if (EdGraph->IsNameUnique(NewName))
	{
		Modify();
		SetEdNodeName(NewName);
		bRenamedNode = true;
	}
	return bRenamedNode;
}
FText UXD_CampGraph_EditorNode::GetEdNodeName() const
{
	return EdNodeName;
}
void UXD_CampGraph_EditorNode::SetEdNodeName(const FText & Name)
{
	EdNodeName = Name;
}

void UXD_CampGraph_EditorNode::SetEdNodeName(const FName & Name)
{
	SetEdNodeName(FText::FromName(Name));
}


#undef LOCTEXT_NAMESPACE