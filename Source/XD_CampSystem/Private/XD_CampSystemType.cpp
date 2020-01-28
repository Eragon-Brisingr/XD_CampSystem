// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampSystemType.h"
#include <Internationalization/StringTable.h>
#include <GameFramework/Actor.h>

#include "XD_CampManager.h"
#include "XD_CampSystemUtility.h"
#include "XD_CampInfo.h"
#include "XD_CampSystemSetting.h"

FXD_CampConfig::FXD_CampConfig()
{

}

FXD_CampConfig::FXD_CampConfig(const UXD_CampInfo* InCampInfo)
	:
	CampGuid(InCampInfo->CampGuid)
#if WITH_EDITORONLY_DATA
	, CampName(InCampInfo->CampName)
#endif
{

}

FXD_CampConfig::FXD_CampConfig(const UObject* WorldContextObject, const FText& CampName)
{
	SetCamp(WorldContextObject, CampName);
}

class UXD_CampInfo* FXD_CampConfig::GetCamp(const UObject* WorldContextObject) const
{
	if (CampInfo == nullptr)
	{
		UXD_CampManager* CampManager = UXD_CampManager::GetCampManager(WorldContextObject);
		CampInfo = CampManager->FindCampByGuid(CampGuid);
		if (CampInfo == nullptr)
		{
			ensure(!CampManager->GetOwner()->HasAuthority());
			return CampManager->GetDefaultCampInfo();
		}
	}
	return CampInfo;
}

void FXD_CampConfig::InitCampInfo(const UObject* WorldContextObject) const
{
	UXD_CampManager* CampManager = UXD_CampManager::GetCampManager(WorldContextObject);
	CampInfo = CampManager->FindCampByGuid(CampGuid);

#if WITH_EDITOR
	CampSystem_Error_Log("阵营[%s]不存在", *CampName.ToString());
#endif
}

bool FXD_CampConfig::SetCamp(const UObject* WorldContextObject, const FText& InCampName)
{
	if (UXD_CampManager* CampManager = UXD_CampManager::GetCampManager(WorldContextObject))
	{
		if (UXD_CampInfo* FindCampInfo = CampManager->FindCampByName(InCampName))
		{
			return SetCamp(FindCampInfo);
		}
	}
	return false;
}

bool FXD_CampConfig::SetCamp(class UXD_CampInfo* InCampInfo)
{
	if (InCampInfo)
	{
		CampInfo = InCampInfo;
#if WITH_EDITORONLY_DATA
		CampName = InCampInfo->CampName;
#endif
		CampGuid = InCampInfo->CampGuid;
		return true;
	}
	return false;
}
