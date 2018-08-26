// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampSystemFunctionLibrary.h"
#include "XD_CampManager.h"
#include "XD_CampInfo.h"




class UXD_CampInfo* UXD_CampSystemFunctionLibrary::GetCamp(FXD_CampConfig& Camp, const UObject* WorldContextObject)
{
	return Camp.GetCamp(WorldContextObject);
}

bool UXD_CampSystemFunctionLibrary::SetCamp(FXD_CampConfig& Camp, const UObject* WorldContextObject, const FText& CampName)
{
	return Camp.SetCamp(WorldContextObject, CampName);
}

bool UXD_CampSystemFunctionLibrary::SetCampByRef(FXD_CampConfig& Camp, class UXD_CampInfo* CampInfo)
{
	return Camp.SetCamp(CampInfo);
}

FXD_CampConfig UXD_CampSystemFunctionLibrary::MakeCampConfig(const UObject* WorldContextObject, const FText& CampName)
{
	return FXD_CampConfig(WorldContextObject, CampName);
}

UXD_CampInfo* UXD_CampSystemFunctionLibrary::BreakCampConfig(const FXD_CampConfig& CampConfig, const UObject* WorldContextObject)
{
	return CampConfig.GetCamp(WorldContextObject);
}

class UXD_CampManager* UXD_CampSystemFunctionLibrary::GetCampManager(const UObject* WorldContextObject)
{
	return UXD_CampManager::GetCampManager(WorldContextObject);
}

bool UXD_CampSystemFunctionLibrary::SetCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, float Value, bool Between)
{
	return GetCampManager(WorldContextObject)->SetCampRelationshipValue(Camp.GetCamp(WorldContextObject), WithCamp.GetCamp(WorldContextObject), Value, Between);
}

bool UXD_CampSystemFunctionLibrary::AddCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, float AddValue, bool Between)
{
	return GetCampManager(WorldContextObject)->AddCampRelationshipValue(Camp.GetCamp(WorldContextObject), WithCamp.GetCamp(WorldContextObject), AddValue, Between);
}

bool UXD_CampSystemFunctionLibrary::ReduceCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, float ReduceValue, bool Between)
{
	return GetCampManager(WorldContextObject)->ReduceCampRelationshipValue(Camp.GetCamp(WorldContextObject), WithCamp.GetCamp(WorldContextObject), ReduceValue, Between);
}

float UXD_CampSystemFunctionLibrary::GetCampRelationshipValue(const UObject* WorldContextObject, const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp)
{
	return GetCampManager(WorldContextObject)->GetCampRelationshipValue(Camp.GetCamp(WorldContextObject), WithCamp.GetCamp(WorldContextObject));
}

EXD_CampRelationship UXD_CampSystemFunctionLibrary::GetCampRelationship(const FXD_CampConfig& Camp, const FXD_CampConfig& WithCamp, const UObject* WorldContextObject)
{
	return GetCampManager(WorldContextObject)->GetCampRelationship(Camp.GetCamp(WorldContextObject), WithCamp.GetCamp(WorldContextObject));
}
