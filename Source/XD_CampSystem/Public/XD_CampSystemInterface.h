// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "XD_CampSystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UXD_CampSystem_GameStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XD_CAMPSYSTEM_API IXD_CampSystem_GameStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "阵营")
	class UXD_CampManager* GetCampManager() const;
	virtual class UXD_CampManager* GetCampManager_Implementation() const { return nullptr; }
	static UXD_CampManager* GetCampManager(UObject* Obj) { return IXD_CampSystem_GameStateInterface::Execute_GetCampManager(Obj); }
};
