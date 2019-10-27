// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "SubclassOf.h"
#include "XD_CampInfo.h"
#include "XD_CampGraph_NewNode_SchemaAction.generated.h"

/**
 *
 */
USTRUCT()
struct FXD_CampGraph_NewNode_SchemaAction : public FEdGraphSchemaAction
{
public:
	GENERATED_USTRUCT_BODY()

	FXD_CampGraph_NewNode_SchemaAction() :FEdGraphSchemaAction(), NewCampInfo(nullptr) {}
	FXD_CampGraph_NewNode_SchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, UXD_CampInfo* InCampInfo) :FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NewCampInfo(InCampInfo) {}

	/** Execute this action, given the graph and schema, and possibly a pin that we were dragged from. Returns a node that was created by this action (if any). */
	UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	UXD_CampInfo* NewCampInfo;
};
