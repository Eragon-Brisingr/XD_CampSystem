// Fill out your copyright notice in the Description page of Project Settings.
#include "GraphEditor/EditorGraph/SchemaActions/XD_CampGraph_NewNode_SchemaAction.h"
#include <EdGraph/EdGraph.h>

#include "GraphEditor/EditorGraph/EditorNodes/XD_CampGraph_EditorNode.h"
#include "XD_CampGraph.h"

#define LOCTEXT_NAMESPACE "XD_CampEditorGraphSchemaAction_NewNode"

UEdGraphNode* FXD_CampGraph_NewNode_SchemaAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin * FromPin, const FVector2D Location, bool bSelectNewNode)
{
	check(ParentGraph);

	ParentGraph->Modify();


	UXD_CampGraph* CampGraph = CastChecked<UXD_CampGraph>(ParentGraph->GetOuter());
	CampGraph->Modify();

	FGraphNodeCreator<UXD_CampGraph_EditorNode> Creator(*ParentGraph);
	UXD_CampGraph_EditorNode* EditorNode = Creator.CreateNode(bSelectNewNode);
	EditorNode->SetCampInfo(NewCampInfo);
	CampGraph->AddCamp(NewCampInfo);
	Creator.Finalize();

    //EditorNode->AllocateDefaultPins();   for some reason it was called 2 times even if I only call it here
    EditorNode->AutowireNewNode(FromPin);
    EditorNode->NodePosX = Location.X;
    EditorNode->NodePosY = Location.Y;

	return EditorNode;
}
#undef LOCTEXT_NAMESPACE