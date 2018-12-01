// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "XD_CampSystemType.h"
#include "Engine/EngineTypes.h"
#include "XD_CampInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class XD_CAMPSYSTEM_API UXD_CampInfo : public UObject
{
	GENERATED_BODY()
public:
	UXD_CampInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

	virtual void ReplicatedCampRelationships(bool& WroteSomething, class UActorChannel * Channel, class FOutBunch * Bunch, FReplicationFlags * RepFlags);
public:
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Replicated, Category = "阵营", meta = (DisplayName = "阵营名"))
	FText CampName;

	//考虑阵营更名情况，使用Guid作为阵营唯一信息
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "阵营", meta = (DisplayName = "阵营GUID"))
	FGuid CampGuid;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Replicated, Category = "阵营", meta = (DisplayName = "阵营关系列表"), Instanced)
	TArray<class UXD_CampRelationship*> CampRelationships;
	
	//假如和别的阵营没有过关系变化，取默认对其他阵营关系
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "阵营", Instanced, meta = (DisplayName = "默认对其他阵营关系"), Replicated, SaveGame)
	class UXD_CampRelationship* DefaultCampRelationship;

	//同阵营关系
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "阵营", Instanced, meta = (DisplayName = "同阵营关系"), Replicated, SaveGame)
	class UXD_CampRelationship* SelfCampRelationship;

private:
	int32 AddCampRelationship(UXD_CampInfo * WithCamp);
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	bool SetCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp, float Value);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	bool AddCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp, float AddValue);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	bool ReduceCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp, float ReduceValue);
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	float GetCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp) const;

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	virtual EXD_CampRelationship GetCampRelationship(const UObject* WorldContextObject, UXD_CampInfo* WithCamp) const;

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", meta = (WorldContext = "WorldContextObject"))
	class UXD_CampRelationship* GetCampRelationshipRef(const UObject* WorldContextObject, UXD_CampInfo* WithCamp) const;
protected:
	virtual EXD_CampRelationship ExplainCampRelationship(float RelationshipValue) const;
public:

};

