// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampEditor_GraphSchema.h"
#include "SubclassOf.h"
#include "XD_CampSystemEditor_Log.h"
#include "XD_CampEditor_ConnectionDrawingPolicy.h"
#include "UObjectIterator.h"
#include "XD_CampGraph_NewNode_SchemaAction.h"
#include "EdGraph/EdGraph.h"
#include "XD_CampGraph.h"
#include "XD_CampGraph_EditorNode.h"
#include "XD_CampEditor_ClassHelper.h"
#include "ModuleManager.h"
#include "XD_CampSystem_Editor.h"

#define LOCTEXT_NAMESPACE "XD_CampEditor_GraphSchema"

UXD_CampEditor_GraphSchema::UXD_CampEditor_GraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void UXD_CampEditor_GraphSchema::GetGraphContextActions(FGraphContextMenuBuilder & ContextMenuBuilder) const
{

	FText ToolTip = LOCTEXT("NewXD_CampEditorGraphNodeTooltip", "Add a {NodeName} to the graph.");
	FText MenuDesc = LOCTEXT("NewXD_CampEditorGraphNodeDescription", "{NodeName}");

	FXD_CampSystem_EditorModule& Module = FModuleManager::LoadModuleChecked<FXD_CampSystem_EditorModule>("XD_CampSystem_Editor");
    TSharedPtr<FXD_CampEditor_ClassHelper> Helper = Module.GetHelper();

	//Gathering C++ classes

	FCategorizedGraphActionListBuilder BaseBuilder(TEXT("C++ Defined Nodes"));

    TArray<FXD_CampEditor_ClassData> AllSubClasses;
    Helper->GatherClasses(UXD_CampNode::StaticClass(),AllSubClasses);

    for (auto& NativeClassData : AllSubClasses)
    {
        if (NativeClassData.GetClass()->HasAnyClassFlags(CLASS_Native))
        {
            FFormatNamedArguments Arguments;
            Arguments.Add(TEXT("NodeName"), NativeClassData.GetClass()->GetDisplayNameText());
            TSharedPtr<FXD_CampGraph_NewNode_SchemaAction> NewNodeAction;

            NewNodeAction = MakeShareable(new FXD_CampGraph_NewNode_SchemaAction(NativeClassData.GetCategory(), FText::Format(MenuDesc, Arguments), FText::Format(ToolTip, Arguments), 0, NativeClassData.GetClass()));

            BaseBuilder.AddAction(NewNodeAction);
        }
    }

	ContextMenuBuilder.Append(BaseBuilder);

	//Gathering child blueprints
    FCategorizedGraphActionListBuilder BlueprintBuilder(TEXT("Blueprint Defined Nodes"));

	for (auto& BlueprintClassData : AllSubClasses)
	{
		if (!BlueprintClassData.GetClass()->HasAnyClassFlags(CLASS_Native))
		{
			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("NodeName"), BlueprintClassData.GetClass()->GetDisplayNameText());
            TSharedPtr<FXD_CampGraph_NewNode_SchemaAction> NewNodeAction;

            NewNodeAction = MakeShareable(new FXD_CampGraph_NewNode_SchemaAction(BlueprintClassData.GetCategory(), FText::Format(MenuDesc, Arguments), FText::Format(ToolTip, Arguments), 0, BlueprintClassData.GetClass()));

            BlueprintBuilder.AddAction(NewNodeAction);
		}
	}

	ContextMenuBuilder.Append(BlueprintBuilder);
}

void UXD_CampEditor_GraphSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	Super::GetContextMenuActions(CurrentGraph, InGraphNode, InGraphPin, MenuBuilder, bIsDebugging);
}

const FPinConnectionResponse UXD_CampEditor_GraphSchema::CanCreateConnection(const UEdGraphPin * A, const UEdGraphPin * B) const
{
	if(!(A && B))
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both pins must be available."));

	if (A->GetOwningNode() == B->GetOwningNode())
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You can't connect a node to itself."));

	if (A->Direction == EGPD_Input && B->Direction == EGPD_Input)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You can't connect an input pin to another input pin."));

	if (A->Direction == EGPD_Output && B->Direction == EGPD_Output)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("You can't connect an output pin to another output pin"));

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

FConnectionDrawingPolicy * UXD_CampEditor_GraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect & InClippingRect, FSlateWindowElementList & InDrawElements, UEdGraph * InGraphObj) const
{
	return new FXD_CampEditor_ConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

void UXD_CampEditor_GraphSchema::CreateDefaultNodesForGraph(UEdGraph & Graph) const
{
	if (Graph.Nodes.Num() == 0)
	{
		UXD_CampGraph* GraphAsset = Cast<UXD_CampGraph>(Graph.GetOuter());

		GraphAsset->Modify();
		Graph.Modify();

		UXD_CampNode* AssetNode = GraphAsset->SpawnNodeInsideGraph<UXD_CampNode>(UXD_CampNode::StaticClass());

		FGraphNodeCreator<UXD_CampGraph_EditorNode>Creator(Graph);
		UXD_CampGraph_EditorNode* EdNode = Creator.CreateNode();
		EdNode->SetAssetNode(AssetNode);
		EdNode->AllocateDefaultPins();

		Creator.Finalize();

		EdNode->NodePosX = 0;
		EdNode->NodePosY = 0;
	}
}
#undef LOCTEXT_NAMESPACE