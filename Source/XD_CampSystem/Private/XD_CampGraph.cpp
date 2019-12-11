// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampGraph.h"
#include "XD_CampInfo.h"
#include "XD_CampSystemSetting.h"
#define LOCTEXT_NAMESPACE "XD_CampGraph"

UXD_CampGraph::UXD_CampGraph()
{
#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
#endif
}

#if WITH_EDITORONLY_DATA

UXD_CampInfo* UXD_CampGraph::CreateCampInfoInsideGraph()
{
	UXD_CampInfo* SpawenNode = NewObject<UXD_CampInfo>(this, GetDefault<UXD_CampSystemSetting>()->CampInfoClass.LoadSynchronous(), NAME_None, RF_Transactional);
	AddCamp(SpawenNode);
	return SpawenNode;
}

void UXD_CampGraph::AddCamp(UXD_CampInfo* InNode)
{
	InNode->SetGraph(this);
	CampList.Add(InNode);
}

void UXD_CampGraph::RemoveCamp(UXD_CampInfo* NodeToRemove)
{
	CampList.Remove(NodeToRemove);
}

TArray<TSharedPtr<FString>> UXD_CampGraph::GetAllCampNames() const
{
	TArray<TSharedPtr<FString>> Res;
	for (UXD_CampInfo* CampInfo : CampList)
	{
		Res.Add(MakeShareable(new FString(CampInfo->CampName.ToString())));
	}
	return Res;
}

#endif

#undef LOCTEXT_NAMESPACE