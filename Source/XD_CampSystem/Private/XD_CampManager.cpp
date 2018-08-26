// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampManager.h"
#include <UnrealNetwork.h>
#include <Engine/ActorChannel.h>
#include <GameFramework/GameStateBase.h>
#include "XD_CampInfo.h"
#include "XD_CampManagerUtility.h"
#include "XD_CampSystemInterface.h"
#include "XD_CampSystemSetting.h"

UXD_CampManager::UXD_CampManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicated(true);
}

void UXD_CampManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UXD_CampManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UXD_CampManager::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UXD_CampManager, CampList);
}

bool UXD_CampManager::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UXD_CampInfo* CampInfo : CampList)
	{
		if (CampInfo)
		{
			WroteSomething |= Channel->ReplicateSubobject(CampInfo, *Bunch, *RepFlags);
			CampInfo->ReplicatedCampRelationships(WroteSomething, Channel, Bunch, RepFlags);
		}
	}

	return WroteSomething;
}

UXD_CampManager* UXD_CampManager::GetCampManager(const UObject* WorldContextObject)
{
	if (AGameStateBase* GameState = WorldContextObject->GetWorld()->GetGameState())
	{
		if (GameState->Implements<UXD_CampSystem_GameStateInterface>())
		{
			return IXD_CampSystem_GameStateInterface::Execute_GetCampManager(GameState);
		}
		else
		{
			CampSystem_Warning_LOG("请在GameState中实现XD_CampSystemInterface接口并实现GetCampManager");
			return GameState->FindComponentByClass<UXD_CampManager>();
		}
	}
	CampSystem_Warning_LOG("请在GameState中添加XD_CampManager组件，实现XD_CampSystemInterface接口并实现GetCampManager");
	return nullptr;
}

bool UXD_CampManager::IsCampExist(const FText& CampName) const
{
	for (UXD_CampInfo* CampInfo : CampList)
	{
		if (CampInfo->CampName.EqualTo(CampName))
		{
			return true;
		}
	}
	return false;
}

void UXD_CampManager::AddCamp(const FText& CampName)
{
	if (IsCampExist(CampName))
	{
		CampSystem_Error_Log("无法添加[%s]，该阵营已存在", *CampName.ToString());
	}
	else
	{
		UXD_CampInfo* CampInfo = NewObject<UXD_CampInfo>(this, GetDefault<UXD_CampSystemSetting>()->CampInfoClass, FName(*CampName.ToString()));
		CampInfo->CampName = CampName;
		CampInfo->CampGuid = FGuid::NewGuid();
		CampList.Add(CampInfo);
		CampSystem_Display_Log("添加[%s]阵营", *CampName.ToString());
	}
}

void UXD_CampManager::RemoveCampByName(const FText& CampName)
{
	int32 Index = CampList.IndexOfByPredicate([&](UXD_CampInfo* CampInfo) {return CampInfo->CampName.EqualTo(CampName); });
	if (Index != INDEX_NONE)
	{
		UXD_CampInfo* NeedRemoveCamp = CampList[Index];

		for (UXD_CampInfo* CampInfo : CampList)
		{
			int32 CampRelationshipIndex = CampInfo->CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->WithCamp.GetCamp(this) == NeedRemoveCamp; });
			if (CampRelationshipIndex != INDEX_NONE)
			{
				CampInfo->CampRelationships.RemoveAt(CampRelationshipIndex);
			}
		}

		CampList.RemoveAt(Index);
	}
	else
	{
		CampSystem_Error_Log("无法移除%s，该阵营名不存在", *CampName.ToString());
	}
}

class UXD_CampInfo* UXD_CampManager::FindCampByName(const FText& CampName) const
{
	int32 Index = CampList.IndexOfByPredicate([&](UXD_CampInfo* CampInfo) {return CampInfo->CampName.EqualTo(CampName); });
	if (Index != INDEX_NONE)
	{
		return CampList[Index];
	}
	else
	{
		CampSystem_Error_Log("无法找到%s，该阵营名不存在", *CampName.ToString());
		return nullptr;
	}
}

class UXD_CampInfo* UXD_CampManager::FindCampByGuid(const FGuid& Guid) const
{
	int32 Index = CampList.IndexOfByPredicate([&](UXD_CampInfo* CampInfo) {return CampInfo->CampGuid == Guid; });
	if (Index != INDEX_NONE)
	{
		return CampList[Index];
	}
	else
	{
		CampSystem_Error_Log("无法找到%s，该阵营GUID不存在", *Guid.ToString());
		return nullptr;
	}
}

bool UXD_CampManager::SetCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp, float Value, bool Between)
{
	if (Camp && WithCamp)
	{
		if (Camp->SetCampRelationshipValue(this, WithCamp, Value))
		{
			if (Between)
			{
				WithCamp->SetCampRelationshipValue(this, Camp, Value);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

bool UXD_CampManager::AddCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp, float AddValue, bool Between)
{
	if (Camp && WithCamp)
	{
		if (Camp->AddCampRelationshipValue(this, WithCamp, AddValue))
		{
			if (Between)
			{
				WithCamp->AddCampRelationshipValue(this, Camp, AddValue);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

bool UXD_CampManager::ReduceCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp, float ReduceValue, bool Between)
{
	if (Camp && WithCamp)
	{
		if (Camp->ReduceCampRelationshipValue(this, WithCamp, ReduceValue))
		{
			if (Between)
			{
				WithCamp->ReduceCampRelationshipValue(this, Camp, ReduceValue);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

float UXD_CampManager::GetCampRelationshipValue(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp) const
{
	return Camp->GetCampRelationshipValue(this, WithCamp);
}

EXD_CampRelationship UXD_CampManager::GetCampRelationship(UXD_CampInfo* Camp, UXD_CampInfo* WithCamp) const
{
	return Camp->GetCampRelationship(this, WithCamp);
}
