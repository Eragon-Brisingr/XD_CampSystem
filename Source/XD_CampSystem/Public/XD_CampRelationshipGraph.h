// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraph/GenericGraph.h"
#include "XD_CampRelationshipGraph.generated.h"

class UXD_CampInfo;
class UStringTable;

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
	TArray<UXD_CampInfo*> CampList;
	
	UPROPERTY(VisibleAnywhere, Category = "阵营配置", meta = (DisplayName = "阵营名表"))
	UStringTable* CampNameStringTable;

	TArray<FText> GetAllCampNames() const;

#if WITH_EDITOR
	virtual void RebuildGenericGraph() override;
#endif
};
