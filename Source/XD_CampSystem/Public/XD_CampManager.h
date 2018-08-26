// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "XD_SaveGameInterface.h"
#include "XD_CampSystemType.h"
#include "XD_CampManager.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class XD_CAMPSYSTEM_API UXD_CampManager : public UActorComponent, public IXD_SaveGameInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXD_CampManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	//阵营
public:
	static UXD_CampManager* GetCampManager(const UObject* WorldContextObject);

 	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly, Category = "阵营", EditFixedSize, Replicated, meta = (DisplayName = "阵营列表"))
 	TArray<class UXD_CampInfo*> CampList;
 
  	UFUNCTION(BlueprintPure, Category = "游戏|阵营")
  	bool IsCampExist(const FText& CampName) const;
  
  	UFUNCTION(BlueprintCallable, Category = "游戏|阵营", BlueprintAuthorityOnly)
  	void AddCamp(const FText& CampName);
	
	//不应该调用比较合适
  	void RemoveCampByName(const FText& CampName);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	class UXD_CampInfo* FindCampByName(const FText& CampName) const;

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	class UXD_CampInfo* FindCampByGuid(const FGuid& Guid) const;

public:
	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	bool SetCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp, float Value, bool Between);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	bool AddCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp, float AddValue, bool Between);

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	bool ReduceCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp, float ReduceValue, bool Between);

public:
	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	float GetCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp) const;

	UFUNCTION(BlueprintCallable, Category = "游戏|阵营")
	EXD_CampRelationship GetCampRelationship(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp) const;
};
