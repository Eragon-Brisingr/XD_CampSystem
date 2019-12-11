// Fill out your copyright notice in the Description page of Project Settings.
#include "GraphEditor/EditorGraph/XD_CampEditor_GraphSchema.h"
#include <Templates/SubclassOf.h>
#include <UObject/UObjectIterator.h>
#include <EdGraph/EdGraph.h>
#include <Modules/ModuleManager.h>
#include <ScopedTransaction.h>
#include <Internationalization/StringTable.h>
#include <Internationalization/StringTableCore.h>

#include "XD_CampInfo.h"
#include "XD_CampGraph.h"
#include "XD_CampSystemSetting.h"
#include "XD_CampSystem_Editor.h"
#include "GraphEditor/Utility/XD_CampSystemEditor_Log.h"
#include "GraphEditor/EditorGraph/XD_CampEditor_ConnectionDrawingPolicy.h"
#include "GraphEditor/EditorGraph/SchemaActions/XD_CampGraph_NewNode_SchemaAction.h"
#include "GraphEditor/EditorGraph/EditorNodes/XD_CampGraph_EditorNode.h"
#include "GraphEditor/Utility/XD_CampEditor_ClassHelper.h"

#define LOCTEXT_NAMESPACE "XD_CampEditor_GraphSchema"

struct FXD_CampGraph_AssetSchemaAction_NewEdge : public FEdGraphSchemaAction
{
public:
	FXD_CampGraph_AssetSchemaAction_NewEdge() : EdgeTemplate(nullptr) {}

	FXD_CampGraph_AssetSchemaAction_NewEdge(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), EdgeTemplate(nullptr) {}

	UXD_CampGraph_EditorEdge* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

	UXD_CampGraph_EditorEdge* EdgeTemplate;
};

UXD_CampGraph_EditorEdge* FXD_CampGraph_AssetSchemaAction_NewEdge::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	UXD_CampGraph_EditorEdge* ResultNode = nullptr;

	if (EdgeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("CampGraphEditorNewEdge", "Camp Graph Editor: New Edge"));
		ParentGraph->Modify();
		if (FromPin != nullptr)
		{
			FromPin->Modify();
		}

		EdgeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(EdgeTemplate, true, bSelectNewNode);

		EdgeTemplate->CreateNewGuid();
		EdgeTemplate->PostPlacedNewNode();
		EdgeTemplate->AllocateDefaultPins();
		EdgeTemplate->AutowireNewNode(FromPin);

		EdgeTemplate->NodePosX = Location.X;
		EdgeTemplate->NodePosY = Location.Y;

		EdgeTemplate->CampRelationship->SetFlags(RF_Transactional);
		EdgeTemplate->SetFlags(RF_Transactional);

		ResultNode = EdgeTemplate;
	}

	return ResultNode;
}

UXD_CampEditor_GraphSchema::UXD_CampEditor_GraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void UXD_CampEditor_GraphSchema::GetGraphContextActions(FGraphContextMenuBuilder & ContextMenuBuilder) const
{
	UXD_CampGraph* CampGraph = CastChecked<UXD_CampGraph>(ContextMenuBuilder.CurrentGraph->GetOuter());

	FText ToolTip = LOCTEXT("NewXD_CampEditorGraphNodeTooltip", "添加阵营 {CampName} 至图表.");
	FText MenuDesc = LOCTEXT("NewXD_CampEditorGraphNodeDescription", "{CampName}");

	UStringTable* CampNameStringTable = CampGraph->CampNameStringTable;
	if (CampNameStringTable == nullptr)
	{
		return;
	}

	TArray<FString> StringTableKeys;
	{
		CampNameStringTable->GetStringTable()->EnumerateSourceStrings([&](const FString& Key, const FString& SourceString)
			{
				if (!CampGraph->CampList.ContainsByPredicate([&](UXD_CampInfo* Camp) {return Camp->CampName.ToString() == SourceString; }))
				{
					StringTableKeys.Add(Key);
				}
				return true;
			});
	}

	for (const FString& Key : StringTableKeys)
	{
		UXD_CampInfo* CampInfo = NewObject<UXD_CampInfo>(CampGraph, GetDefault<UXD_CampSystemSetting>()->CampInfoClass.LoadSynchronous(), NAME_None, RF_Transactional);
		CampInfo->CampName = FText::FromStringTable(CampNameStringTable->GetStringTableId(), Key);
		CampInfo->CampGuid = FGuid::NewGuid();

		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("CampName"), CampInfo->CampName);
		TSharedPtr<FXD_CampGraph_NewNode_SchemaAction> NewNodeAction;

		NewNodeAction = MakeShareable(new FXD_CampGraph_NewNode_SchemaAction(LOCTEXT("创建阵营", "创建阵营"), FText::Format(MenuDesc, Arguments), FText::Format(ToolTip, Arguments), 0, CampInfo));

		ContextMenuBuilder.AddAction(NewNodeAction);
	}
}

const FPinConnectionResponse UXD_CampEditor_GraphSchema::CanCreateConnection(const UEdGraphPin * A, const UEdGraphPin * B) const
{
	if(!(A && B))
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both pins must be available."));

	if (A->GetOwningNode() == B->GetOwningNode())
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("不能连接同一节点"));

	if (A->Direction == EGPD_Input && B->Direction == EGPD_Input)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("不能将输入与输入连接"));

	if (A->Direction == EGPD_Output && B->Direction == EGPD_Output)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("不能将输出与输出连接"));

	if (A->LinkedTo.ContainsByPredicate([&](UEdGraphPin* E)
		{
			return E->GetOwningNode()->GetAllPins().ContainsByPredicate([&](UEdGraphPin* E2) {return E2->LinkedTo.Contains(B); });
		}))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("不能重复连接节点", "不能重复连接节点"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, TEXT(""));
}

bool UXD_CampEditor_GraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UXD_CampGraph_EditorNode* NodeA = Cast<UXD_CampGraph_EditorNode>(A->GetOwningNode());
	UXD_CampGraph_EditorNode* NodeB = Cast<UXD_CampGraph_EditorNode>(B->GetOwningNode());

	if (NodeA == nullptr || NodeB == nullptr)
		return false;

	UEdGraph* Graph = NodeA->GetGraph();
	FVector2D InitPos((NodeA->NodePosX + NodeB->NodePosX) / 2, (NodeA->NodePosY + NodeB->NodePosY) / 2);

	FXD_CampGraph_AssetSchemaAction_NewEdge Action;
 	Action.EdgeTemplate = NewObject<UXD_CampGraph_EditorEdge>(Graph);
 	Action.EdgeTemplate->SetEdge(A->Direction == EGPD_Output ? NodeA->CreateConnectEdge() : NodeB->CreateConnectEdge());
	UXD_CampGraph_EditorEdge* EdgeNode = Action.PerformAction(NodeA->GetGraph(), nullptr, InitPos, false);
 
 	if (A->Direction == EGPD_Output)
 	{
 		EdgeNode->CreateConnections(A, B);
 	}
 	else
 	{
 		EdgeNode->CreateConnections(B, A);
 	}

	return true;
}

FConnectionDrawingPolicy * UXD_CampEditor_GraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect & InClippingRect, FSlateWindowElementList & InDrawElements, UEdGraph * InGraphObj) const
{
	return new FXD_CampEditor_ConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

void UXD_CampEditor_GraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{

}
#undef LOCTEXT_NAMESPACE