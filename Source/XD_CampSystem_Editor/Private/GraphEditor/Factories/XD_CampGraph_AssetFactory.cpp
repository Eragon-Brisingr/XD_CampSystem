// Fill out your copyright notice in the Description page of Project Settings.
#include "GraphEditor/Factories/XD_CampGraph_AssetFactory.h"
#include "XD_CampGraph.h"

UXD_CampGraph_AssetFactory::UXD_CampGraph_AssetFactory()
{
	SupportedClass = UXD_CampGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UXD_CampGraph_AssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UXD_CampGraph>(InParent, InClass, InName, Flags);
}