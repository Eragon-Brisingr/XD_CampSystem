// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampSystemType.h"
#include "XD_CampManager.h"
#include "XD_CampSystemUtility.h"
#include "XD_CampInfo.h"
#include "XD_CampSystemSetting.h"
#include <StringTable.h>

FXD_CampConfig::FXD_CampConfig()
{
#if WITH_EDITOR
	if (UStringTable* CampNameStringTable = GetDefault<UXD_CampSystemSetting>()->GetCampNameStringTable())
	{
		CampName = FText::FromStringTable(CampNameStringTable->GetStringTableId(), FString());
	}
#endif
}

FXD_CampConfig::FXD_CampConfig(const UObject* WorldContextObject, const FText& CampName)
{
	SetCamp(WorldContextObject, CampName);
}

class UXD_CampInfo* FXD_CampConfig::GetCamp(const UObject* WorldContextObject) const
{
	if (CampInfo == nullptr)
	{
		if (UXD_CampManager* CampManager = UXD_CampManager::GetCampManager(WorldContextObject))
		{
			if (CampGuid.IsValid())
			{
				CampInfo = CampManager->FindCampByGuid(CampGuid);
			}
			else if (!CampName.IsEmpty())
			{
				CampInfo = CampManager->FindCampByName(CampName);
			}
		}
	}
	return CampInfo;
}

bool FXD_CampConfig::SetCamp(const UObject* WorldContextObject, const FText& CampName)
{
	if (UXD_CampManager* CampManager = UXD_CampManager::GetCampManager(WorldContextObject))
	{
		if (UXD_CampInfo* CampInfo = CampManager->FindCampByName(CampName))
		{
			this->CampInfo = CampInfo;
			this->CampName = CampName;
			this->CampGuid = CampInfo->CampGuid;
			return true;
		}
	}
	CampSystem_Error_Log("设置Camp为%s无效", *CampName.ToString());
	return false;
}

bool FXD_CampConfig::SetCamp(class UXD_CampInfo* CampInfo)
{
	if (CampInfo)
	{
		this->CampInfo = CampInfo;
		CampName = CampInfo->CampName;
		CampGuid = CampInfo->CampGuid;
		return true;
	}
	return false;
}
