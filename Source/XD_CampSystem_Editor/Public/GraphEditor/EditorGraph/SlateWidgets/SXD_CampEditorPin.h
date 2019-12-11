// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include <SGraphPin.h>

#define LOCTEXT_NAMESPACE "SXD_CampEditorGraphGraphPin"

/**
 *
 */
class SXD_CampEditorPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SXD_CampEditorPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	virtual FSlateColor GetPinColor() const override;

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

	const FSlateBrush* GetPinBorder() const;
};

#undef LOCTEXT_NAMESPACE