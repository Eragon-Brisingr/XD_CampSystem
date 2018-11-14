﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_CampSystemType.h"
#include "XD_CampRelationship.generated.h"

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
	UPROPERTY(SaveGame, BlueprintReadOnly, Replicated, Category = "阵营关系", meta = (DisplayName = "与阵营"))
	class UXD_CampInfo* ToCamp;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Replicated, Category = "阵营关系", meta = (DisplayName = "阵营友好值", UIMin = "-150", UIMax = "150"))
	float RelationshipValue;
};
