// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampEditorGraph.h"
#include "XD_CampInfo.h"
#include "XD_CampGraph_EditorNode.h"
#include "XD_CampSystemEditor_Log.h"
#include "EdGraph/EdGraphPin.h"
#include "XD_CampGraph.h"

bool UXD_CampEditorGraph::IsNameUnique(const FText & InName)
{
	bool bUnique = true;
	for (UEdGraphNode* Node : Nodes)
	{
		UXD_CampGraph_EditorNode* PNode = Cast<UXD_CampGraph_EditorNode>(Node);
		FText NodeName = PNode->GetEdNodeName();
		if (bUnique==true && !NodeName.IsEmpty())
		{
			if (NodeName.EqualToCaseIgnored(InName))
			{
				CampSystemEditor_Warning_Log("I found another node with the same name: %s.", *NodeName.ToString());
				bUnique = false;
			}
		}
	}
	return bUnique;
}

void UXD_CampEditorGraph::SaveGraph()
{

}

void UXD_CampEditorGraph::RefreshNodes()
{
	for (UEdGraphNode* Node : Nodes)
	{
		if (UXD_CampGraph_EditorNode* XD_CampNode = Cast<UXD_CampGraph_EditorNode>(Node))
		{
			XD_CampNode->UpdateVisualNode();
		}
	}
}

UXD_CampGraph * UXD_CampEditorGraph::GetGraphAsset()
{
	return Cast<UXD_CampGraph>(GetOuter());
}
