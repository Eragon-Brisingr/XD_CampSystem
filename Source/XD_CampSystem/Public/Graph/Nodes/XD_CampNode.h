// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_CampNode.generated.h"

#define LOCTEXT_NAMESPACE "XD_CampNode"

class UXD_CampGraph;
/**
 * Base abstract class for every node of the graph.
 */
UCLASS(Blueprintable, BlueprintType)
class XD_CAMPSYSTEM_API UXD_CampNode : public UObject
{
	GENERATED_BODY()

public:
	UXD_CampNode();

	UFUNCTION(BlueprintPure, Category = "Node Data")
	TArray<UXD_CampNode*> GetChildren();

	UFUNCTION(BlueprintPure, Category = "Node Data")
	TArray<UXD_CampNode*> GetParents();

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Node Data")
	FText GetNodeTitle();
    virtual FText GetNodeTitle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Node Data")
	bool HasInputPins();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Node Data")
	bool HasOutputPins();

	UFUNCTION(BlueprintNativeEvent, Category = "Node Data")
	UXD_CampNode* GetNodePointer();
    virtual UXD_CampNode* GetNodePointer_Implementation();


	virtual void SetGraph(UXD_CampGraph* InGraph);
	virtual UXD_CampGraph* GetGraph();
	virtual void AddToChildren(UXD_CampNode* NewChildNode);
	virtual void AddToParent(UXD_CampNode* NewParentNode);
	virtual bool HasParentNodes();
	virtual void LinkArgumentNodeAsChild(UXD_CampNode* Child);
	virtual void ClearLinks();
	virtual bool RemoveLinkedNode(UXD_CampNode* NodeToRemove);
	virtual bool RemoveNodeFromParents(UXD_CampNode* NodeToRemove);
	virtual bool RemoveNodeFromChilds(UXD_CampNode* NodeToRemove);

protected:
	UPROPERTY()
	UXD_CampGraph * Graph = nullptr;
	UPROPERTY()
	TArray<UXD_CampNode*> ParentNodes;
	UPROPERTY()
	TArray<UXD_CampNode*> ChildNodes;
    UPROPERTY()
	bool bHasInputPins = true;
    UPROPERTY()
	bool bHasOutputPins = true;
    UPROPERTY()
	FText DefaultNodeTitle = FText();


};

#undef LOCTEXT_NAMESPACE