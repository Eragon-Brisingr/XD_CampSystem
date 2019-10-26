// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "XD_CampNode.h"
#include "SubclassOf.h"
#include "XD_CampGraph_EditorNode.generated.h"

/**
 *
 */
UCLASS()
class UXD_CampGraph_EditorNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()

public:
	// Inherited via EdGraphNode.h
	TSharedPtr<SGraphNode> CreateVisualWidget() override; 	/** Create a visual widget to represent this node in a graph editor or graph panel.  If not implemented, the default node factory will be used. */
	void AllocateDefaultPins() override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	void PrepareForCopying() override;
	void DestroyNode() override;
	void AutowireNewNode(UEdGraphPin* FromPin) override;
	void GetContextMenuActions(const FGraphNodeContextMenuBuilder& Context) const override;

public:
	virtual void SetAssetNode(UXD_CampNode* InNode);
	virtual UXD_CampNode* GetAssetNode();

	virtual void PostCopyNode();

	virtual bool RenameUniqueNode(const FText& NewName);

	virtual FText GetEdNodeName() const;
	virtual void SetEdNodeName(const FText& Name);
	virtual void SetEdNodeName(const FName& Name);

	virtual TSharedPtr<SWidget> GetContentWidget();

	virtual void UpdateVisualNode();

    virtual void SaveNodesAsChildren(TArray<UEdGraphNode*>& Children);
protected:
	virtual bool HasOutputPins();
	virtual bool HasInputPins();
	TSharedPtr<SGraphNode>SlateNode;
public:
	UPROPERTY(Instanced)
	UXD_CampNode* AssetNode = nullptr;
protected:
	UPROPERTY()
	FText EdNodeName;
};
