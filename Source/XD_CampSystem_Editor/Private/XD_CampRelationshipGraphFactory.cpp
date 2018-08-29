// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampRelationshipGraphFactory.h"
#include "XD_CampRelationshipGraph.h"

#define LOCTEXT_NAMESPACE "XD_CampSystem_Editor"

 UXD_CampRelationshipGraphFactory::UXD_CampRelationshipGraphFactory()
 {
 	bCreateNew = true;
 	bEditAfterNew = true;
 	SupportedClass = UXD_CampRelationshipGraph::StaticClass();
 }
 
 UObject* UXD_CampRelationshipGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
 {
 	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
 }
 
 FText UXD_CampRelationshipGraphFactory::GetDisplayName() const
 {
 	return LOCTEXT("阵营关系图表", "阵营关系图表");
 }
 
 FString UXD_CampRelationshipGraphFactory::GetDefaultNewAssetName() const
 {
 	return TEXT("阵营关系图表");
 }

#undef LOCTEXT_NAMESPACE
