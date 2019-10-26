// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampEditorGraph.h"
#include "XD_CampNode.h"
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
	LinkAssetNodes();
	MapNamedNodes();
}

void UXD_CampEditorGraph::ClearOldLinks()
{
	for (UEdGraphNode* EditorNode : Nodes)
	{
		UXD_CampGraph_EditorNode* EdNode = Cast<UXD_CampGraph_EditorNode>(EditorNode);
		if (EdNode)
			EdNode->AssetNode->ClearLinks();
	}
}

void UXD_CampEditorGraph::LinkAssetNodes()
{
	ClearOldLinks();
	CampSystemEditor_Log("Starting to link all asset nodes from the editor graph links.");
	for (UEdGraphNode* EditorNode : Nodes)
	{
		if (UXD_CampGraph_EditorNode* EdNode = Cast<UXD_CampGraph_EditorNode>(EditorNode))
		{
			UXD_CampNode* NodeAsset = EdNode->AssetNode;
			if (NodeAsset != nullptr)
			{

				TArray<UEdGraphPin*>& EdPinsParent = EdNode->Pins;
				TArray<UEdGraphNode*>Children;

				for (UEdGraphPin* Pin : EdPinsParent)
				{
					//Take only the output pins
					if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
					{

						TArray<UEdGraphPin*>& EdPinsChildren = Pin->LinkedTo;
						for (UEdGraphPin* LinkedPin : EdPinsChildren)
						    Children.Add(LinkedPin->GetOwningNode());

					}

				}

                EdNode->SaveNodesAsChildren(Children);
			}
			else
			{
				CampSystemEditor_Error_Log("There is no asset node linked to this editor node.");
			}
		}
		else
		{
			CampSystemEditor_Warning_Log("An unknow EdNode has been found.");
		}
	}

}

void UXD_CampEditorGraph::RefreshNodes()
{
	for (UEdGraphNode* Node : Nodes)
	{
		if (UXD_CampGraph_EditorNode* XD_CampNode = Cast<UXD_CampGraph_EditorNode>(Node))
			XD_CampNode->UpdateVisualNode();
	}
}

void UXD_CampEditorGraph::MapNamedNodes()
{
	UXD_CampGraph* Graph = GetGraphAsset();
	Graph->NamedNodes.Empty();
    Graph->NodesNames.Empty();

	for (UEdGraphNode* Node : Nodes)
	{
		if (UXD_CampGraph_EditorNode* XD_CampNode = Cast<UXD_CampGraph_EditorNode>(Node))
		{
			FText Name = XD_CampNode->GetEdNodeName();
            if (!Name.IsEmpty())
            {
                Graph->NamedNodes.Add(Name.ToString(), XD_CampNode->AssetNode);
                Graph->NodesNames.Add(XD_CampNode->AssetNode, Name.ToString());
            }
		}
	}
}

UXD_CampGraph * UXD_CampEditorGraph::GetGraphAsset()
{
	return Cast<UXD_CampGraph>(GetOuter());
}
