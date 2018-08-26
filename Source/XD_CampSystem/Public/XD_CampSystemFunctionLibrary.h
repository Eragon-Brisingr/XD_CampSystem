// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XD_CampSystemType.h"
#include "XD_CampSystemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XD_CAMPSYSTEM_API UXD_CampSystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static class UXD_CampInfo* GetCamp(UPARAM(Ref)FXD_CampConfig& Camp, const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static bool SetCamp(UPARAM(Ref)FXD_CampConfig& Camp, const UObject* WorldContextObject, const FText& CampName);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static bool SetCampByRef(UPARAM(Ref)FXD_CampConfig& Camp, class UXD_CampInfo* CampInfo);

	UFUNCTION(BlueprintPure, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static FXD_CampConfig MakeCampConfig(const UObject* WorldContextObject, const FText& CampName);

	UFUNCTION(BlueprintPure, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static class UXD_CampInfo* BreakCampConfig(const FXD_CampConfig& CampConfig, const UObject* WorldContextObject);
public:
	UFUNCTION(BlueprintPure, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static class UXD_CampManager* GetCampManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static bool SetCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, float Value, bool Between);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static bool AddCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, float AddValue, bool Between);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static bool ReduceCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, float ReduceValue, bool Between);

	UFUNCTION(BlueprintPure, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static float GetCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp);
	
	UFUNCTION(BlueprintPure, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	static EXD_CampRelationship GetCampRelationship(const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, const UObject* WorldContextObject);
};
