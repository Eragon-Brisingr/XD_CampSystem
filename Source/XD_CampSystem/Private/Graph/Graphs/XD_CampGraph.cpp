// Fill out your copyright notice in the Description page of Project Settings.
#include "XD_CampGraph.h"
#include "XD_CampNode.h"
#define LOCTEXT_NAMESPACE "XD_CampGraph"

UXD_CampGraph::UXD_CampGraph()
{

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
#endif

}

void UXD_CampGraph::InitGraph(UObject * ParentObject)
{
    Owner = ParentObject;
}

#if WITH_EDITORONLY_DATA
void UXD_CampGraph::AddNode(UXD_CampNode * InNode)
{
	InNode->SetGraph(this);
	InNode->Rename(nullptr, this);
	int32 Index = Nodes.Add(InNode);
}

void UXD_CampGraph::RemoveNode(UXD_CampNode * NodeToRemove)
{
	for (UXD_CampNode* Node : Nodes)
	{
		Node->RemoveLinkedNode(NodeToRemove);
	}

	int32 Removed=Nodes.Remove(NodeToRemove);
}
#endif

#undef LOCTEXT_NAMESPACE