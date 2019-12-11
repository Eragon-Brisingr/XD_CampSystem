// Fill out your copyright notice in the Description page of Project Settings.
#include "GraphEditor/EditorGraph/SlateWidgets/SXD_CampEditorPin.h"
#include "GraphEditor/Utility/XD_CampGraph_EditorStyle.h"


void SXD_CampEditorPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Default);

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != nullptr);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	SBorder::Construct(SBorder::FArguments()
		.BorderImage(this, &SXD_CampEditorPin::GetPinBorder)
		.BorderBackgroundColor(this, &SXD_CampEditorPin::GetPinColor)
		.OnMouseButtonDown(this, &SXD_CampEditorPin::OnPinMouseDown)
		.Cursor(this, &SXD_CampEditorPin::GetPinCursor)
		.Padding(FMargin(10.0f))
	);
}

FSlateColor SXD_CampEditorPin::GetPinColor() const
{
	return XD_CampGraph_EditorStyle::Pin::Default;
}

TSharedRef<SWidget> SXD_CampEditorPin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

const FSlateBrush* SXD_CampEditorPin::GetPinBorder() const
{
	return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
}
