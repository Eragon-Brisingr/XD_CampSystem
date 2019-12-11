// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/NoExportTypes.h>
#include "XD_CampSystemType.h"
#include "XD_CampRelationship.generated.h"

class UXD_CampInfo;

/**
 * 
 */
UCLASS(collapsecategories)
class XD_CAMPSYSTEM_API UXD_CampRelationship : public UObject
{
	GENERATED_BODY()
public:
	UXD_CampRelationship();

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

public:
	UPROPERTY(SaveGame, BlueprintReadOnly, Replicated, Category = "阵营", meta = (DisplayName = "与阵营"))
	UXD_CampInfo* ToCamp;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Replicated, Category = "阵营", meta = (DisplayName = "关系值", UIMin = "-150", UIMax = "150"))
	float RelationshipValue;

	UXD_CampInfo* GetOwningCamp() const;
};
