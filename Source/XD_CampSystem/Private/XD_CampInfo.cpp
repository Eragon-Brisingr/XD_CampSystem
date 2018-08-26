// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampInfo.h"
#include <UnrealNetwork.h>
#include <Engine/ActorChannel.h>
#include "XD_CampRelationship.h"
#include "XD_CampManagerUtility.h"
#include "XD_CampSystemSetting.h"




UXD_CampInfo::UXD_CampInfo()
{

}

void UXD_CampInfo::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}

	DOREPLIFETIME(UXD_CampInfo, CampName);
	DOREPLIFETIME(UXD_CampInfo, CampRelationships);
}

void UXD_CampInfo::ReplicatedCampRelationships(bool& WroteSomething, class UActorChannel * Channel, class FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	for (UXD_CampRelationship* CampRelationship : CampRelationships)
	{
		if (CampRelationship)
		{
			WroteSomething |= Channel->ReplicateSubobject(CampRelationship, *Bunch, *RepFlags);
		}
	}
}

int32 UXD_CampInfo::AddCampRelationship(UXD_CampInfo * WithCamp)
{
	UXD_CampRelationship* CampRelationship = NewObject<UXD_CampRelationship>(GetOuter(), GetDefault<UXD_CampSystemSetting>()->CampRelationshipClass, *FString::Printf(TEXT("%s对%s阵营关系"), *CampName.ToString(), *WithCamp->CampName.ToString()));
	CampRelationship->WithCamp.SetCamp(WithCamp);
	return CampRelationships.Add(CampRelationship);
}

bool UXD_CampInfo::SetCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp, float Value)
{
	if (WithCamp)
	{
		if (this == WithCamp)
		{
			CampSystem_Display_Log("设置的阵营[%s]与本阵营为相同阵营，不能设置阵营间关系", *WithCamp->CampName.ToString());
			return false;
		}
		int32 Index = CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->WithCamp.GetCamp(WorldContextObject) == WithCamp; });
		if (Index == INDEX_NONE)
		{
			Index = AddCampRelationship(WithCamp);
		}
		CampRelationships[Index]->RelationshipValue = Value;
		CampSystem_Display_Log("设置阵营[%s]对阵营[%s]关系为[%f]", *CampName.ToString(), *WithCamp->CampName.ToString(), Value);
		return true;
	}
	return false;
}

bool UXD_CampInfo::AddCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp, float AddValue)
{
	if (WithCamp && AddValue > 0.f)
	{
		if (this == WithCamp)
		{
			CampSystem_Display_Log("设置的阵营[%s]与本阵营为相同阵营，不能设置阵营间关系", *WithCamp->CampName.ToString());
			return false;
		}
		int32 Index = CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->WithCamp.GetCamp(WorldContextObject) == WithCamp; });
		if (Index == INDEX_NONE)
		{
			Index = AddCampRelationship(WithCamp);
		}
		CampRelationships[Index]->RelationshipValue += AddValue;
		CampSystem_Display_Log("阵营[%s]对阵营[%s]关系提升[%f]，现在为[%f]", *CampName.ToString(), *WithCamp->CampName.ToString(), AddValue, CampRelationships[Index]->RelationshipValue);
		return true;
	}
	return false;
}

bool UXD_CampInfo::ReduceCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp, float ReduceValue)
{
	if (WithCamp && ReduceValue > 0.f)
	{
		if (this == WithCamp)
		{
			CampSystem_Display_Log("设置的阵营[%s]与本阵营为相同阵营，不能设置阵营间关系", *WithCamp->CampName.ToString());
			return false;
		}
		int32 Index = CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->WithCamp.GetCamp(WorldContextObject) == WithCamp; });
		if (Index == INDEX_NONE)
		{
			Index = AddCampRelationship(WithCamp);
		}
		CampRelationships[Index]->RelationshipValue -= ReduceValue;
		CampSystem_Display_Log("阵营[%s]对阵营[%s]关系降低[%f]，现在为[%f]", *CampName.ToString(), *WithCamp->CampName.ToString(), ReduceValue, CampRelationships[Index]->RelationshipValue);
		return true;
	}
	return false;
}

float UXD_CampInfo::GetCampRelationshipValue(const UObject* WorldContextObject, UXD_CampInfo* WithCamp) const
{
	if (WithCamp)
	{
		if (this == WithCamp)
		{
			return SelfCampRelationship;
		}
		int32 Index = CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->WithCamp.GetCamp(WorldContextObject) == WithCamp; });
		if (Index != INDEX_NONE)
		{
			return CampRelationships[Index]->RelationshipValue;
		}
	}
	return DefaultCampRelationship;
}

EXD_CampRelationship UXD_CampInfo::GetCampRelationship(const UObject* WorldContextObject, UXD_CampInfo* WithCamp) const
{
	if (WithCamp)
	{
		if (this == WithCamp)
		{
			return EXD_CampRelationship::SelfCamp;
		}
		int32 Index = CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->WithCamp.GetCamp(WorldContextObject) == WithCamp; });
		if (Index != INDEX_NONE)
		{
			float RelationshipValue = CampRelationships[Index]->RelationshipValue;
			return ExplainCampRelationship(RelationshipValue);
		}
	}
	return ExplainCampRelationship(DefaultCampRelationship);
}

EXD_CampRelationship UXD_CampInfo::ExplainCampRelationship(float RelationshipValue) const
{
	if (RelationshipValue > 50.f)
	{
		return EXD_CampRelationship::Friend;
	}
	else if (RelationshipValue < -50.f)
	{
		return EXD_CampRelationship::Hostile;
	}
	else
	{
		return EXD_CampRelationship::Neutral;
	}
}
