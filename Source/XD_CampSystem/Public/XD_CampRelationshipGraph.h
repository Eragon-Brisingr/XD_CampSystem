// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph/GenericGraph.h"
#include "XD_CampRelationshipGraph.generated.h"

/**
 * 
 */
UCLASS()
class XD_CAMPSYSTEM_API UXD_CampRelationshipGraph : public UGenericGraph
{
	GENERATED_BODY()
public:
	UXD_CampRelationshipGraph();
	
	UPROPERTY()
	TArray<class UXD_CampInfo*> CampList;
	
#if WITH_EDITOR
	virtual void RebuildGenericGraph() override;
#endif
};
