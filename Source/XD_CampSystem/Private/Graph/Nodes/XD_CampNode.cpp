// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampNode.h"
#include "XD_CampGraph.h"

#define LOCTEXT_NAMESPACE "XD_CampNode"

UXD_CampNode::UXD_CampNode()
{
	DefaultNodeTitle = LOCTEXT("Node Title", "Node");
}

UXD_CampNode * UXD_CampNode::GetNodePointer_Implementation()
{
	return this;
}

void UXD_CampNode::SetGraph(UXD_CampGraph * InGraph)
{
	Graph = InGraph;
}

UXD_CampGraph * UXD_CampNode::GetGraph()
{
	return Graph;
}

FText UXD_CampNode::GetNodeTitle_Implementation()
{
	return DefaultNodeTitle.IsEmpty() ? LOCTEXT("Node Title", "Node") : DefaultNodeTitle;
}

TArray<UXD_CampNode*> UXD_CampNode::GetChildren()
{
	TArray<UXD_CampNode*>ReturnArray;
	for (UXD_CampNode* Node : ChildNodes)
	{
        auto* object = Node->GetNodePointer();
		if(object)ReturnArray.Add(object);
	}
	return ReturnArray;
}

TArray<UXD_CampNode*> UXD_CampNode::GetParents()
{
	return ParentNodes;
}

bool UXD_CampNode::HasInputPins_Implementation()
{
	return bHasInputPins;
}

bool UXD_CampNode::HasOutputPins_Implementation()
{
	return bHasOutputPins;
}

void UXD_CampNode::AddToChildren(UXD_CampNode * NewSubNode)
{
	ChildNodes.Add(NewSubNode);
}

void UXD_CampNode::AddToParent(UXD_CampNode * NewParentNode)
{
	ParentNodes.Add(NewParentNode);
}

bool UXD_CampNode::HasParentNodes()
{
	return ParentNodes.Num() > 0;
}

void UXD_CampNode::LinkArgumentNodeAsChild(UXD_CampNode * Child)
{
	AddToChildren(Child);
	Child->AddToParent(this);
}

void UXD_CampNode::ClearLinks()
{
	ParentNodes.Empty();
	ChildNodes.Empty();
}

bool UXD_CampNode::RemoveLinkedNode(UXD_CampNode * NodeToRemove)
{
	return RemoveNodeFromParents(NodeToRemove) || RemoveNodeFromChilds(NodeToRemove);
}

bool UXD_CampNode::RemoveNodeFromParents(UXD_CampNode * NodeToRemove)
{
	return ParentNodes.Remove(NodeToRemove);
}

bool UXD_CampNode::RemoveNodeFromChilds(UXD_CampNode * NodeToRemove)
{
	return ChildNodes.Remove(NodeToRemove);
}

#undef LOCTEXT_NAMESPACE