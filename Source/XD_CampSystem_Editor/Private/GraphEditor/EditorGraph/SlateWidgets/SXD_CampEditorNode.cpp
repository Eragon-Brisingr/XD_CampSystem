// Fill out your copyright notice in the Description page of Project Settings.
#include "SXD_CampEditorNode.h"
#include "Slate.h"
#include "XD_CampGraph_EditorNode.h"
#include "SGraphPin.h"
#include "SXD_CampEditorPin.h"
#include "XD_CampSystemEditor_Log.h"
#include "SCommentBubble.h"
#include "GraphEditorSettings.h"

#define LOCTEXT_NAMESPACE "SXD_CampGraphNode"

void SXD_CampEditorNode::Construct(const FArguments & InArgs, UEdGraphNode * InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SXD_CampEditorNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	TSharedPtr<SErrorText> ErrorText;

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(FMargin(1.f, 5.0f))
			.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				[
					SNew(SBorder)
                    .BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
                    .Padding(FMargin(5.0f))
                    .BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.4f))
                    [
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.HAlign(HAlign_Center)
						.AutoHeight()
						[
							SAssignNew(InlineEditableText, SInlineEditableTextBlock)
							.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
							.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
							.IsReadOnly(this, &SXD_CampEditorNode::IsNameReadOnly)
							.OnTextCommitted(this, &SXD_CampEditorNode::OnNameTextCommited)
							.OnVerifyTextChanged(this, &SXD_CampEditorNode::OnVerifyNameTextChanged)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							NodeTitle.ToSharedRef()
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(ContentWidget, SBox)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							// 错误提示
							SAssignNew(ErrorText, SErrorText)
							.BackgroundColor(this, &SXD_CampEditorNode::GetErrorColor)
							.ToolTipText(this, &SXD_CampEditorNode::GetErrorMsgToolTip)
						]
                    ]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
		];

	//注释用的气泡
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	UpdateErrorInfo();
	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);

	CreatePinWidgets();
	CreateContent();
}

void SXD_CampEditorNode::CreatePinWidgets()
{
	UXD_CampGraph_EditorNode* EdNode = CastChecked<UXD_CampGraph_EditorNode>(GraphNode);
	for (int32 i = 0; i < EdNode->Pins.Num(); ++i)
	{
		UEdGraphPin* Pin = EdNode->Pins[i];
		if (!Pin->bHidden)
		{
			TSharedPtr<SGraphPin>NewPin = SNew(SXD_CampEditorPin, Pin);
			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SXD_CampEditorNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(0.f, 0.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else
	{
		RightNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(0.f, 0.0f)
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

bool SXD_CampEditorNode::IsNameReadOnly() const
{
	return false;
}

void SXD_CampEditorNode::CreateContent()
{
	UXD_CampGraph_EditorNode* Node = Cast<UXD_CampGraph_EditorNode>(GraphNode);

	ContentWidget->SetContent(Node->GetContentWidget());
	ContentWidget->SetMinDesiredWidth(200.f);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
#undef LOCTEXT_NAMESPACE