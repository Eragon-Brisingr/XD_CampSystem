// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "XD_CampInfo.h"
#include "XD_CampEditor_GraphSchema.generated.h"

class UXD_CampGraph_EditorEdge;

/**
 *
 */
UCLASS()
class UXD_CampEditor_GraphSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

public:
	void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;

	class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;

	void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
};