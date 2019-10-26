// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "SubclassOf.h"
#include "XD_CampNode.h"
#include "XD_CampGraph_NewNode_SchemaAction.generated.h"

/**
 *
 */
USTRUCT()
struct FXD_CampGraph_NewNode_SchemaAction : public FEdGraphSchemaAction
{
public:
	GENERATED_USTRUCT_BODY()

	FXD_CampGraph_NewNode_SchemaAction() :FEdGraphSchemaAction(), NewNodeClass(nullptr) {}
	FXD_CampGraph_NewNode_SchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, TSubclassOf<UXD_CampNode> InNodeClass) :FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NewNodeClass(InNodeClass) {}

	/** Execute this action, given the graph and schema, and possibly a pin that we were dragged from. Returns a node that was created by this action (if any). */
	UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

    virtual UEdGraphNode* CreateEditorNode(UEdGraph * ParentGraph, bool bSelectNewNode, UXD_CampNode* AssetNode);
	TSubclassOf<UXD_CampNode>NewNodeClass;


};
