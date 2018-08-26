// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_CampSystemType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EXD_CampRelationship :uint8
{
	SelfCamp UMETA(DisplayName = "同阵营"),
	Friend UMETA(DisplayName = "友好"),
	Neutral UMETA(DisplayName = "中立"),
	Hostile UMETA(DisplayName = "敌对")
};

//持久化所属阵营信息
USTRUCT(BlueprintType, meta = (HasNativeMake = "XD_CampSystemFunctionLibrary.MakeCampConfig", HasNativeBreak = "XD_CampSystemFunctionLibrary.BreakCampConfig"))
struct FXD_CampConfig
{
	GENERATED_BODY()

public:
	FXD_CampConfig();
	FXD_CampConfig(const UObject* WorldContextObject, const FText& CampName);

	UPROPERTY(EditAnywhere, Category = "阵营", SaveGame, NotReplicated)
	FText CampName;

	//考虑阵营更名情况，使用Guid作为阵营唯一信息
	UPROPERTY(VisibleAnywhere, Category = "阵营", SaveGame, NotReplicated)
	FGuid CampGuid;
private:
	UPROPERTY(VisibleAnywhere, Instanced, Category = "阵营")
	mutable class UXD_CampInfo* CampInfo;

public:
	class UXD_CampInfo* GetCamp(const UObject* WorldContextObject) const;

	bool SetCamp(const UObject* WorldContextObject, const FText& CampName);

	bool SetCamp(class UXD_CampInfo* CampInfo);
};
