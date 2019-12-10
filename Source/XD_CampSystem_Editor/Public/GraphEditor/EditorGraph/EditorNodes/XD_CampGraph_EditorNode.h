// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "XD_CampInfo.h"
#include "SubclassOf.h"
#include "XD_CampGraph_EditorNode.generated.h"

class UXD_CampRelationship;

/**
 *
 */
UCLASS(collapsecategories)
class UXD_CampGraph_EditorNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()

public:
	// Inherited via EdGraphNode.h
	TSharedPtr<SGraphNode> CreateVisualWidget() override;
	void AllocateDefaultPins() override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	void PrepareForCopying() override;
	void DestroyNode() override;
	void AutowireNewNode(UEdGraphPin* FromPin) override;
	void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
public:
	void SetCampInfo(UXD_CampInfo* InCampInfo);
	UXD_CampInfo* GetAssetNode();

	void PostCopyNode();

	bool RenameUniqueNode(const FText& NewName);

	FText GetEdNodeName() const;
	void SetEdNodeName(const FText& Name);
	void SetEdNodeName(const FName& Name);

	TSharedRef<SWidget> GetContentWidget();

	void UpdateVisualNode();

	UXD_CampRelationship* CreateConnectEdge() const;
protected:
	TSharedPtr<SGraphNode> SlateNode;
public:
	UPROPERTY(VisibleAnywhere, Instanced)
	UXD_CampInfo* CampInfo = nullptr;
protected:
	UPROPERTY()
	FText EdNodeName;
};

UCLASS(collapsecategories)
class UXD_CampGraph_EditorEdge : public UEdGraphNode
{
	GENERATED_BODY()
public:
	TSharedPtr<SGraphNode> CreateVisualWidget() override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	void AllocateDefaultPins() override;
	void PinConnectionListChanged(UEdGraphPin* Pin) override;

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
protected:
	TSharedPtr<SGraphNode> SlateNode;
public:
	UPROPERTY(VisibleAnywhere, Instanced)
	UXD_CampRelationship* CampRelationship = nullptr;

	void SetEdge(UXD_CampRelationship* InCampEdge);
	void CreateConnections(UEdGraphPin* From, UEdGraphPin* To);

	UXD_CampGraph_EditorNode* GetOwingCampNode() const;
	UXD_CampGraph_EditorNode* GetToCampNode() const;

	TSharedRef<SWidget> GetContentWidget();

	FLinearColor GetBackgroundColor() const;
	FLinearColor GetConnectionColor() const;
};