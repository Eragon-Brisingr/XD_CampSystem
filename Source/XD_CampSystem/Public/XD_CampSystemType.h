// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/NoExportTypes.h>
#include "XD_CampSystemType.generated.h"

class UXD_CampInfo;
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

// 持久化所属阵营信息
USTRUCT(BlueprintType) // Pyhton模块报错 meta = (HasNativeMake = "XD_CampSystemFunctionLibrary.MakeCampConfig", HasNativeBreak = "XD_CampSystemFunctionLibrary.BreakCampConfig")
struct XD_CAMPSYSTEM_API FXD_CampConfig
{
	GENERATED_BODY()
public:
	FXD_CampConfig();
	FXD_CampConfig(const UXD_CampInfo* InCampInfo);
	FXD_CampConfig(const UObject* WorldContextObject, const FText& CampName);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "阵营")
	mutable UXD_CampInfo* CampInfo;

public:
	// 考虑阵营更名情况，使用Guid作为阵营唯一信息
	UPROPERTY(VisibleAnywhere, Category = "阵营", SaveGame, NotReplicated)
	FGuid CampGuid;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "阵营", NotReplicated)
	FText CampName;
#endif
public:
	UXD_CampInfo* GetCamp(const UObject* WorldContextObject) const;

	void InitCampInfo(const UObject* WorldContextObject) const;
	bool SetCamp(const UObject* WorldContextObject, const FText& CampName);
	bool SetCamp(class UXD_CampInfo* CampInfo);
};
