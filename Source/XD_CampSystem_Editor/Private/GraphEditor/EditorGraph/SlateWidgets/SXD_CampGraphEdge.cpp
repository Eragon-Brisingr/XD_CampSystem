#include "GraphEditor/EditorGraph/SlateWidgets/SXD_CampGraphEdge.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SToolTip.h"
#include <SGraphPanel.h>
#include <EdGraphSchema_K2.h>
#include <SCommentBubble.h>
#include "GraphEditor/EditorGraph/EditorNodes/XD_CampGraph_EditorNode.h"
#include "ConnectionDrawingPolicy.h"

#define LOCTEXT_NAMESPACE "SGenericGraphEdge"

void SXD_CampGraphEdge::Construct(const FArguments& InArgs, UXD_CampGraph_EditorEdge* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

bool SXD_CampGraphEdge::RequiresSecondPassLayout() const
{
	return true;
}

void SXD_CampGraphEdge::PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const
{
	UXD_CampGraph_EditorEdge* EdgeNode = CastChecked<UXD_CampGraph_EditorEdge>(GraphNode);

	FGeometry StartGeom;
	FGeometry EndGeom;

 	UXD_CampGraph_EditorNode* Start = EdgeNode->GetOwingCampNode();
 	UXD_CampGraph_EditorNode* End = EdgeNode->GetToCampNode();
 	if (Start != nullptr && End != nullptr)
 	{
 		const TSharedRef<SNode>* pFromWidget = NodeToWidgetLookup.Find(Start);
 		const TSharedRef<SNode>* pToWidget = NodeToWidgetLookup.Find(End);
 		if (pFromWidget != nullptr && pToWidget != nullptr)
 		{
 			const TSharedRef<SNode>& FromWidget = *pFromWidget;
 			const TSharedRef<SNode>& ToWidget = *pToWidget;
 
 			StartGeom = FGeometry(FVector2D(Start->NodePosX, Start->NodePosY), FVector2D::ZeroVector, FromWidget->GetDesiredSize(), 1.0f);
 			EndGeom = FGeometry(FVector2D(End->NodePosX, End->NodePosY), FVector2D::ZeroVector, ToWidget->GetDesiredSize(), 1.0f);
 		}
 	}

	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, 0, 1);
}

void SXD_CampGraphEdge::UpdateGraphNode()
{
	UXD_CampGraph_EditorEdge* EdgeNode = CastChecked<UXD_CampGraph_EditorEdge>(GraphNode);

	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	TSharedPtr<SErrorText> ErrorText;

	this->ContentScale.Bind( this, &SGraphNode::GetContentScale );
	this->GetOrAddSlot( ENodeZone::Center )
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.BorderBackgroundColor(this, &SXD_CampGraphEdge::GetBackgroundColor)
			.Padding(2.f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
				.BorderBackgroundColor(FLinearColor::Black)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.Visibility(EVisibility::SelfHitTestInvisible)
				.Padding(FMargin(2.f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text_Lambda([=]() {return EdgeNode->GetNodeTitle(ENodeTitleType::Type::FullTitle); })
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						EdgeNode->GetContentWidget()
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						// 错误提示
						SAssignNew(ErrorText, SErrorText)
						.BackgroundColor(this, &SXD_CampGraphEdge::GetErrorColor)
						.ToolTipText(this, &SXD_CampGraphEdge::GetErrorMsgToolTip)
					]
				]
			]
		];

	// Create comment bubble
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

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
}

void SXD_CampGraphEdge::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	const FVector2D DesiredNodeSize = GetDesiredSize();

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 10.f;

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + ((DesiredNodeSize / 2.f + Height) * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();
	
	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.

	const float MutliNodeSpace = 0.2f; // Space between multiple transition nodes (in units of <size of node> )
	const float MultiNodeStep = (1.f + MutliNodeSpace); //Step between node centres (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

FSlateColor SXD_CampGraphEdge::GetBackgroundColor() const
{
	UXD_CampGraph_EditorEdge* EdgeNode = CastChecked<UXD_CampGraph_EditorEdge>(GraphNode);
	return EdgeNode->GetBackgroundColor();
}

#undef LOCTEXT_NAMESPACE
