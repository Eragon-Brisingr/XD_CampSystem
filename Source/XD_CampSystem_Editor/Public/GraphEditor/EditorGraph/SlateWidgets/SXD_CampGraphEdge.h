#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SNodePanel.h"
#include "SGraphNode.h"
#include "NotifyHook.h"

class SToolTip;
class UXD_CampGraph_EditorEdge;

class SXD_CampGraphEdge : public SGraphNode, public FNotifyHook
{
public:
	SLATE_BEGIN_ARGS(SXD_CampGraphEdge){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UXD_CampGraph_EditorEdge* InNode);

	bool RequiresSecondPassLayout() const override;
	void PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const override;

	virtual void UpdateGraphNode() override;

	// Calculate position for multiple nodes to be placed between a start and end point, by providing this nodes index and max expected nodes 
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;
protected:
	FSlateColor GetBackgroundColor() const;
private:
	TSharedPtr<STextEntryPopup> TextEntryWidget;
};
