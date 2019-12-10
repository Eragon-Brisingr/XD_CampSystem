// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampGraph_EditorNode.h"
#include "XD_CampInfo.h"
#include "XD_CampGraph.h"
#include "XD_CampEditorGraph.h"
#include "SXD_CampEditorNode.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "ModuleManager.h"
#include "XD_CampSystemEditor_Log.h"
#include "GenericCommands.h"
#include "GraphEditorActions.h"
#include "SXD_CampGraphEdge.h"
#include "XD_CampRelationship.h"
#include "XD_CampSystemSetting.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"

#define LOCTEXT_NAMESPACE "XD_CampGraph_EditorNode"

UXD_CampGraph_EditorNode::UXD_CampGraph_EditorNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

namespace CampGraphPinNames
{
	const FName DefualtPinType(TEXT("Default"));
	const FName InPinName(TEXT("InPin"));
	const FName OutPinName(TEXT("OutPin"));
}

TSharedPtr<SGraphNode> UXD_CampGraph_EditorNode::CreateVisualWidget()
{
	SlateNode= SNew(SXD_CampEditorNode, this);
	return SlateNode;
}

TSharedRef<SWidget> UXD_CampGraph_EditorNode::GetContentWidget()
{
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> View = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	View->SetObject(this);
	return View;
}

void UXD_CampGraph_EditorNode::UpdateVisualNode()
{
	SlateNode->UpdateGraphNode();
}

UXD_CampRelationship* UXD_CampGraph_EditorNode::CreateConnectEdge() const
{
	return NewObject<UXD_CampRelationship>(CampInfo, GetDefault<UXD_CampSystemSetting>()->CampRelationshipClass.LoadSynchronous(), NAME_None, RF_Transactional);
}

void UXD_CampGraph_EditorNode::AllocateDefaultPins()
{
	UEdGraphNode::AllocateDefaultPins();
	CreatePin(EGPD_Input, CampGraphPinNames::DefualtPinType, CampGraphPinNames::InPinName);
	CreatePin(EGPD_Output, CampGraphPinNames::DefualtPinType, CampGraphPinNames::OutPinName);
}

FText UXD_CampGraph_EditorNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return CampInfo ? CampInfo->CampName : GetEdNodeName();
}

void UXD_CampGraph_EditorNode::PrepareForCopying()
{
	if (CampInfo)
	{
		CampInfo->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
	}
	Super::PrepareForCopying();
}

void UXD_CampGraph_EditorNode::DestroyNode()
{
	if (CampInfo)
	{
		CampInfo->GetGraph()->RemoveCamp(CampInfo);
		CampInfo->ConditionalBeginDestroy();
		CampInfo = nullptr;
	}
	Super::DestroyNode();
}

void UXD_CampGraph_EditorNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (FromPin)
	{
		Super::AutowireNewNode(FromPin);
		if (FromPin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			if (GetSchema()->TryCreateConnection(FromPin, FindPin(CampGraphPinNames::OutPinName)))
			{
				FromPin->GetOwningNode()->NodeConnectionListChanged();
			}
		}
		if (FromPin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			if (GetSchema()->TryCreateConnection(FromPin, FindPin(CampGraphPinNames::InPinName)))
			{
				FromPin->GetOwningNode()->NodeConnectionListChanged();
			}
		}
	}
}

void UXD_CampGraph_EditorNode::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("CampGraph_NodeContext"), LOCTEXT("NodeActionsMenuHeader", "节点操作"));
	{
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);

		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}
}

void UXD_CampGraph_EditorNode::SetCampInfo(UXD_CampInfo* InCampInfo)
{
	CampInfo = InCampInfo;
}

UXD_CampInfo* UXD_CampGraph_EditorNode::GetAssetNode()
{
	return CampInfo;
}

void UXD_CampGraph_EditorNode::PostCopyNode()
{
	if (CampInfo)
	{
		UEdGraph* EdGraph = GetGraph();
		UObject* ParentAsset = EdGraph ? EdGraph->GetOuter() : nullptr;
		CampInfo->Rename(nullptr, ParentAsset, REN_DontCreateRedirectors | REN_DoNotDirty);
		CampInfo->ClearFlags(RF_Transient);
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

TSharedPtr<SGraphNode> UXD_CampGraph_EditorEdge::CreateVisualWidget()
{
	SlateNode = SNew(SXD_CampGraphEdge, this);
	return SlateNode;
}

FText UXD_CampGraph_EditorEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::Format(LOCTEXT("CampGraph Editor Edge Title", "[{0}] 对 [{1}]"), GetOwingCampNode()->CampInfo->CampName, GetToCampNode()->CampInfo->CampName);
}

void UXD_CampGraph_EditorEdge::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	UEdGraphPin* Input = CreatePin(EGPD_Input, CampGraphPinNames::DefualtPinType, CampGraphPinNames::InPinName);
	Input->bHidden = true;
	UEdGraphPin* Output = CreatePin(EGPD_Output, CampGraphPinNames::DefualtPinType, CampGraphPinNames::OutPinName);
	Output->bHidden = true;
}

void UXD_CampGraph_EditorEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (Pin->LinkedTo.Num() == 0)
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		UXD_CampInfo* OwingCamp = CampRelationship->GetOwningCamp();
		OwingCamp->CampRelationships.Remove(CampRelationship);

		DestroyNode();
	}
}

void UXD_CampGraph_EditorEdge::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UXD_CampGraph_EditorEdge::SetEdge(UXD_CampRelationship* InCampEdge)
{
	CampRelationship = InCampEdge;
}

void UXD_CampGraph_EditorEdge::CreateConnections(UEdGraphPin* From, UEdGraphPin* To)
{
	Pins[0]->MakeLinkTo(From);
	Pins[1]->MakeLinkTo(To);

	UXD_CampGraph_EditorNode* FromNode = CastChecked<UXD_CampGraph_EditorNode>(From->GetOwningNode());
	UXD_CampInfo* FromCamp = FromNode->CampInfo;
	check(CampRelationship->GetOwningCamp() == FromCamp);

	UXD_CampGraph_EditorNode* ToNode = CastChecked<UXD_CampGraph_EditorNode>(To->GetOwningNode());
	UXD_CampInfo* ToCamp = ToNode->CampInfo;

	check(!FromCamp->CampRelationships.Contains(CampRelationship));
	CampRelationship->ToCamp = ToCamp;
	FromCamp->CampRelationships.Add(CampRelationship);
}

UXD_CampGraph_EditorNode* UXD_CampGraph_EditorEdge::GetOwingCampNode() const
{
	return CastChecked<UXD_CampGraph_EditorNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
}

UXD_CampGraph_EditorNode* UXD_CampGraph_EditorEdge::GetToCampNode() const
{
	return CastChecked<UXD_CampGraph_EditorNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
}

TSharedRef<SWidget> UXD_CampGraph_EditorEdge::GetContentWidget()
{
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> View = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	View->SetObject(this);
	return View;
}

FLinearColor UXD_CampGraph_EditorEdge::GetBackgroundColor() const
{
	return GetConnectionColor();
}

FLinearColor UXD_CampGraph_EditorEdge::GetConnectionColor() const
{
	if (CampRelationship)
	{
		float Weight = CampRelationship->RelationshipValue / 150.f;
		if (Weight > 0)
		{
			return FMath::Lerp(FLinearColor::White, FLinearColor::Green, Weight);
		}
		else
		{
			return FMath::Lerp(FLinearColor::White, FLinearColor::Red, -Weight);
		}
	}
	return FLinearColor::Gray;
}

#undef LOCTEXT_NAMESPACE