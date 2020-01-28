// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampManager.h"
#include <Net/UnrealNetwork.h>
#include <Engine/ActorChannel.h>
#include <GameFramework/GameStateBase.h>
#include "XD_CampInfo.h"
#include "XD_CampSystemUtility.h"
#include "XD_CampSystemInterface.h"
#include "XD_CampSystemSetting.h"
#include "XD_ObjectFunctionLibrary.h"
#include "XD_CampRelationship.h"
#include "XD_CampGraph.h"

#define LOCTEXT_NAMESPACE "阵营"

UXD_CampManager::UXD_CampManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicatedByDefault(true);
}

void UXD_CampManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UXD_CampManager::PostInitProperties()
{
	Super::PostInitProperties();
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

void UXD_CampManager::WhenGameInit_Implementation()
{
	UXD_CampGraph* CampGraph = GetDefault<UXD_CampSystemSetting>()->GlobalCampGraph.LoadSynchronous();
	if (CampGraph)
	{
		// 复制阵营指向并重置Outer
		for (UXD_CampInfo* CampInfo : CampGraph->CampList)
		{
			CampList.Add(UXD_ObjectFunctionLibrary::DuplicateObject(CampInfo, this));
		}
		// 修复CampRelationship指向
		for (UXD_CampInfo* CampInfo : CampList)
		{
			for (UXD_CampRelationship*& CampRelationship : CampInfo->CampRelationships)
			{
				CampRelationship = UXD_ObjectFunctionLibrary::DuplicateObject(CampRelationship, this);
				CampRelationship->ToCamp = *CampList.FindByPredicate([&](UXD_CampInfo* Camp) {return Camp->CampName.EqualTo(CampRelationship->ToCamp->CampName); });
			}
		}
	}
}

UXD_CampManager* UXD_CampManager::GetCampManager(const UObject* WorldContextObject)
{
	if (AGameStateBase* GameState = WorldContextObject->GetWorld()->GetGameState())
	{
		if (GameState->Implements<UXD_CampSystem_GameStateInterface>())
		{
			UXD_CampManager* CampManager = IXD_CampSystem_GameStateInterface::GetCampManager(GameState);
			check(CampManager);
			return CampManager;
		}
		else
		{
			CampSystem_Warning_Log("请在GameState中实现XD_CampSystemInterface接口并实现GetCampManager");
			return GameState->FindComponentByClass<UXD_CampManager>();
		}
	}
	CampSystem_Warning_Log("请在GameState中添加XD_CampManager组件，实现XD_CampSystemInterface接口并实现GetCampManager");
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
		UXD_CampInfo* CampInfo = NewObject<UXD_CampInfo>(this, GetDefault<UXD_CampSystemSetting>()->CampInfoClass.LoadSynchronous(), FName(*CampName.ToString()));
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
			int32 CampRelationshipIndex = CampInfo->CampRelationships.IndexOfByPredicate([&](UXD_CampRelationship* CampRelationship) {return CampRelationship->ToCamp == NeedRemoveCamp; });
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
			return true;
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
			return true;
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
			return true;
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

#undef LOCTEXT_NAMESPACE
