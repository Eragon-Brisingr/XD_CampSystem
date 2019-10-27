// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "XD_CampGraph_AssetFactory.generated.h"

/**
 *
 */
UCLASS()
class UXD_CampGraph_AssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UXD_CampGraph_AssetFactory();
	/**
	* Create a new object by class.
	*
	* @param InClass
	* @param InParent
	* @param InName
	* @param Flags
	* @param Context
	* @param Warn
	* @return The new object.
	*/
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
