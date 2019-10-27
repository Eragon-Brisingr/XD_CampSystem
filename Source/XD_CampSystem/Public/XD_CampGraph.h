// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubclassOf.h"
#include "XD_CampGraph.generated.h"

#define LOCTEXT_NAMESPACE "XD_CampGraph"

class UXD_CampInfo;
class UStartNode;
class UXD_CampInfo;

/**
 *
 */
UCLASS()
class XD_CAMPSYSTEM_API UXD_CampGraph : public UObject
{
	GENERATED_BODY()

public:
	UXD_CampGraph();

	UPROPERTY(VisibleAnywhere)
	TArray<UXD_CampInfo*> CampList;
	
	UPROPERTY(EditAnywhere, Category = "阵营配置", meta = (DisplayName = "阵营名表"))
	UStringTable* CampNameStringTable;

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY()
	class UEdGraph* EdGraph;

	UXD_CampInfo* CreateCampInfoInsideGraph();

	void AddCamp(UXD_CampInfo* InNode);
	void RemoveCamp(UXD_CampInfo* NodeToRemove);

	TArray<TSharedPtr<FString>> GetAllCampNames() const;
#endif
};

#undef LOCTEXT_NAMESPACE