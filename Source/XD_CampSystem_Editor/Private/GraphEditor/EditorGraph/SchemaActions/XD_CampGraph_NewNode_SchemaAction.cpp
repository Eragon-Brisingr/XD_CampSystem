// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampGraph_NewNode_SchemaAction.h"
#include "XD_CampGraph.h"
#include "XD_CampGraph_EditorNode.h"
#include "EdGraph/EdGraph.h"

#define LOCTEXT_NAMESPACE "XD_CampEditorGraphSchemaAction_NewNode"

UEdGraphNode* FXD_CampGraph_NewNode_SchemaAction::PerformAction(UEdGraph * ParentGraph, UEdGraphPin * FromPin, const FVector2D Location, bool bSelectNewNode)
{
	check(ParentGraph);

	ParentGraph->Modify();


	UXD_CampGraph* GraphAsset = CastChecked<UXD_CampGraph>(ParentGraph->GetOuter());
	GraphAsset->Modify();

	UXD_CampNode* AssetNode = GraphAsset->SpawnNodeInsideGraph<UXD_CampNode>(NewNodeClass);

    UEdGraphNode* EditorNode=CreateEditorNode(ParentGraph,bSelectNewNode,AssetNode);

    //EditorNode->AllocateDefaultPins();   for some reason it was called 2 times even if I only call it here
    EditorNode->AutowireNewNode(FromPin);
    EditorNode->NodePosX = Location.X;
    EditorNode->NodePosY = Location.Y;

	return EditorNode;
}
UEdGraphNode * FXD_CampGraph_NewNode_SchemaAction::CreateEditorNode(UEdGraph * ParentGraph, bool bSelectNewNode, UXD_CampNode* AssetNode)
{
    FGraphNodeCreator<UXD_CampGraph_EditorNode>Creator(*ParentGraph);
    UXD_CampGraph_EditorNode* EdNode = Creator.CreateNode(bSelectNewNode);
    EdNode->SetAssetNode(AssetNode);
    Creator.Finalize();
    return EdNode;
}
#undef LOCTEXT_NAMESPACE