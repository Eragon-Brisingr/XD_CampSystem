// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include <EdGraph/EdGraph.h>
#include "XD_CampEditorGraph.generated.h"

/**
 *
 */
class UXD_CampGraph;

UCLASS()
class UXD_CampEditorGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	virtual bool IsNameUnique(const FText& InName);
	virtual void SaveGraph();
	virtual void RefreshNodes();
	virtual UXD_CampGraph* GetGraphAsset();
};
