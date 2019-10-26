// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
DECLARE_LOG_CATEGORY_EXTERN(Log_CampSystemEditor, Verbose, All);
#define CampSystemEditor_Log(Message,...) UE_LOG(Log_CampSystemEditor, Log, TEXT(Message), ##__VA_ARGS__)
#define CampSystemEditor_Display_Log(Message,...) UE_LOG(Log_CampSystemEditor, Display, TEXT(Message), ##__VA_ARGS__)
#define CampSystemEditor_Warning_Log(Message,...) UE_LOG(Log_CampSystemEditor, Warning, TEXT(Message), ##__VA_ARGS__)
#define CampSystemEditor_Error_Log(Message,...) UE_LOG(Log_CampSystemEditor, Error, TEXT(Message), ##__VA_ARGS__)


