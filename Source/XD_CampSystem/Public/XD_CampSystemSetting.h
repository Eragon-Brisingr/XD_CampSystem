// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_CampSystemSetting.generated.h"

/**
 * 
 */
UCLASS(Config = "XD_CampSystemSetting", defaultconfig)
class XD_CAMPSYSTEM_API UXD_CampSystemSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UXD_CampSystemSetting();

	UPROPERTY(EditAnywhere, Category = "阵营设置", Config)
	TSubclassOf<class UXD_CampInfo> CampInfoClass;
	
	UPROPERTY(EditAnywhere, Category = "阵营设置", Config)
	TSubclassOf<class UXD_CampRelationship> CampRelationshipClass;

	UPROPERTY(EditAnywhere, Category = "阵营设置", Config, meta = (DisplayName = "阵营关系图表"))
	TSoftObjectPtr<class UXD_CampRelationshipGraph> CampRelationshipGrap;

	class UStringTable* GetCampNameStringTable() const;
};
