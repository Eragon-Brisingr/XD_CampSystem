// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampRelationshipGraph.h"
#include "XD_CampRelationshipGraph_Impl.h"
#include "XD_ObjectFunctionLibrary.h"
#include "XD_CampRelationship.h"
#include "XD_CampInfo.h"




UXD_CampRelationshipGraph::UXD_CampRelationshipGraph()
{
#if WITH_EDITOR
	NodeType = UXD_CampRelationshipGraph_Node::StaticClass();
#endif
}

#if WITH_EDITOR

void UXD_CampRelationshipGraph::RebuildGenericGraph()
{
	CampList.Empty();

	for (UGenericGraphNode* Node : AllNodes_Editor)
	{
		if (UXD_CampRelationshipGraph_Node* CampNode = Cast<UXD_CampRelationshipGraph_Node>(Node))
		{
			if (CampNode->CampInfo)
			{
				CampList.Add(CampNode->CampInfo);
			}

			for (UXD_CampRelationshipGraph_Edge* CampEdge : CampNode->GetEdgesByType<UXD_CampRelationshipGraph_Edge>())
			{
				//若选择同等关系，则复制一份关系并反转指向
				if (CampEdge && CampEdge->CampRelationship && CampEdge->bRelationshipTwoWay)
				{
					if (UXD_CampRelationshipGraph_Node* ToCampNode = Cast<UXD_CampRelationshipGraph_Node>(CampEdge->EndNode))
					{
						if (ToCampNode->CampInfo)
						{
							UXD_CampRelationship* CampRelationship = UXD_ObjectFunctionLibrary::DuplicateObject(CampEdge->CampRelationship, this);
							CampRelationship->ToCamp = CampNode->CampInfo;
							ToCampNode->CampInfo->CampRelationships.Add(CampRelationship);
						}
					}
				}
			}
		}
	}
}

#endif
