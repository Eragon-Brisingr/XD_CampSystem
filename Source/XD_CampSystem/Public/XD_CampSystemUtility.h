// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "CoreMinimal.h"
// #include <UObject/NoExportTypes.h>
// #include "XD_CampManagerUtility.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(XD_CampSystem_Log, Log, All);
#define CampSystem_Display_Log(Format, ...) UE_LOG(XD_CampSystem_Log, Log, TEXT(Format), ##__VA_ARGS__)
#define CampSystem_Warning_Log(Format, ...) UE_LOG(XD_CampSystem_Log, Warning, TEXT(Format), ##__VA_ARGS__)
#define CampSystem_Error_Log(Format, ...) UE_LOG(XD_CampSystem_Log, Error, TEXT(Format), ##__VA_ARGS__)
