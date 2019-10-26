// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubclassOf.h"
#include "XD_CampGraph.generated.h"

#define LOCTEXT_NAMESPACE "XD_CampGraph"

class UXD_CampNode;
class UStartNode;

/**
 *
 */
UCLASS(Blueprintable,BlueprintType)
class XD_CAMPSYSTEM_API UXD_CampGraph : public UObject
{
	GENERATED_BODY()

public:
	UXD_CampGraph();

	UPROPERTY(BlueprintReadOnly, Category = "Graph Data")
	TArray<UXD_CampNode*> Nodes;

	UPROPERTY(BlueprintReadOnly, Category = "Graph Data")
	TMap<FString, UXD_CampNode*> NamedNodes;

    UPROPERTY(BlueprintReadOnly, Category = "Graph Data")
    TMap<UXD_CampNode*, FString> NodesNames;

    UPROPERTY(BlueprintReadWrite, Category = "Graph Data")
    UObject* Owner;

    UFUNCTION(BlueprintCallable, Category = "Graph Data")
    void InitGraph(UObject* ParentObject);

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY()
	class UEdGraph* EdGraph;

	template <class T>
	T* SpawnNodeInsideGraph(TSubclassOf<UXD_CampNode> InNodeClass)
	{
		T* SpawenNode = NewObject<T>(this, InNodeClass, NAME_None, RF_Transactional);
		AddNode(SpawenNode);
		return SpawenNode;
	}

	virtual void AddNode(UXD_CampNode* InNode);
	virtual void RemoveNode(UXD_CampNode* NodeToRemove);
#endif
};

#undef LOCTEXT_NAMESPACE