// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "NotifyHook.h"
#include "IDetailsView.h"

/**
 *
 */
class SXD_CampEditorNode : public SGraphNode, public FNotifyHook
{
public:
	SLATE_BEGIN_ARGS(SXD_CampEditorNode) {}
	SLATE_END_ARGS()

	// Inherited via SGraphNode
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);
	void UpdateGraphNode() override;
	void CreatePinWidgets() override;
	void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	bool IsNameReadOnly() const override;
protected:
	TSharedPtr<SBox> ContentWidget;

	virtual void CreateContent();
};
