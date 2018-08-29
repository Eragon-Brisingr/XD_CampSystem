// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GenericGraphEdge.h"
#include "XD_CampRelationshipGraph_Impl.generated.h"

/**
 * 
 */
UCLASS()
class XD_CAMPSYSTEM_API UXD_CampRelationshipGraph_Node : public UGenericGraphNode
{
	GENERATED_BODY()
public:
	UXD_CampRelationshipGraph_Node();

	UPROPERTY(VisibleAnywhere, Category = "阵营", Instanced)
	class UXD_CampInfo* CampInfo;

	virtual FText GetNodeTitle_Implementation() const override;
	
	virtual FText GetDescription_Implementation() const override;

	virtual TArray<FGenericGraph_NewNodeParamers> GetNodeTemplates_Implementation(UObject* Outer) const;

	virtual TArray<FGenericGraph_NewEdgeParamers> GetEdgeTemplates_Implementation(UObject* Outer) const;

	virtual void CheckNodeError_Implementation(EGenericGraphNodeCheckMessageType& GenericGraphNodeCheckMessageType, FString& Message, const FGenericGraphNodeErrorCollector& ErrorCollector) const override;

	virtual bool CanCreateConnectionWithOtherNode_Implementation(UGenericGraphNode* Other, FText& ErrorMessage) const override;
};

UCLASS()
class XD_CAMPSYSTEM_API UXD_CampRelationshipGraph_Edge : public UGenericGraphEdge
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "阵营关系", Instanced)
	class UXD_CampRelationship* CampRelationship;

	//使用相同的关系配置
	UPROPERTY(EditAnywhere, Category = "阵营关系", meta = (DisplayName = "同等关系"))
	uint8 bRelationshipTwoWay : 1;

	virtual FText GetDescription_Implementation() const override;

	virtual void WhenGraphRebuild_Implementation() override;

	virtual bool IsBothDirectionConnection_Implementation() const override { return bRelationshipTwoWay; }

	virtual FLinearColor GetConnectionColor_Implementation() const override;

	virtual void CheckNodeError_Implementation(EGenericGraphNodeCheckMessageType& GenericGraphNodeCheckMessageType, FString& Message, const FGenericGraphNodeErrorCollector& ErrorCollector) const override;
};

