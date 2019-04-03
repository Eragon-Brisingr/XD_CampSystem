﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampSystemType.h"
#include "XD_CampManager.h"
#include "XD_CampSystemUtility.h"
#include "XD_CampInfo.h"
#include "XD_CampSystemSetting.h"
#include <StringTable.h>

FXD_CampConfig::FXD_CampConfig()
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

bool FXD_CampConfig::SetCamp(const UObject* WorldContextObject, const FText& InCampName)
{
	if (UXD_CampManager* CampManager = UXD_CampManager::GetCampManager(WorldContextObject))
	{
		if (UXD_CampInfo* NewCampInfo = CampManager->FindCampByName(InCampName))
		{
			CampInfo = NewCampInfo;
			CampName = InCampName;
			CampGuid = NewCampInfo->CampGuid;
			return true;
		}
	}
	CampSystem_Error_Log("设置Camp为%s无效", *InCampName.ToString());
	return false;
}

bool FXD_CampConfig::SetCamp(class UXD_CampInfo* InCampInfo)
{
	if (InCampInfo)
	{
		CampInfo = InCampInfo;
		CampName = InCampInfo->CampName;
		CampGuid = InCampInfo->CampGuid;
		return true;
	}
	return false;
}
