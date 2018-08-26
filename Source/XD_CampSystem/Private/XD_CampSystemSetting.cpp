// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampSystemSetting.h"
#include "XD_CampInfo.h"
#include "XD_CampRelationship.h"




UXD_CampSystemSetting::UXD_CampSystemSetting()
{
	CampInfoClass = UXD_CampInfo::StaticClass();
	CampRelationshipClass = UXD_CampRelationship::StaticClass();
}
