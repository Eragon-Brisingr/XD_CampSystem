// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampRelationshipGraph_Impl.h"
#include "XD_CampInfo.h"
#include "XD_CampSystemSetting.h"
#include "XD_CampRelationship.h"
#include "XD_CampRelationshipGraph.h"
#include "XD_ObjectFunctionLibrary.h"
#include <StringTable.h>
#include <StringTableCore.h>


#define LOCTEXT_NAMESPACE "XD_CampSystem"

UXD_CampRelationshipGraph_Node::UXD_CampRelationshipGraph_Node()
{

}

FText UXD_CampRelationshipGraph_Node::GetNodeTitle_Implementation() const
{
	return CampInfo ? CampInfo->CampName : LOCTEXT("不存在阵营信息", "不存在阵营信息");
}

FText UXD_CampRelationshipGraph_Node::GetDescription_Implementation() const
{
	return CampInfo ? FText::FromString(UXD_ObjectFunctionLibrary::GetObjectPropertysDesc(CampInfo, UObject::StaticClass())) : FText::GetEmpty();
}

TArray<FGenericGraph_NewNodeParamers> UXD_CampRelationshipGraph_Node::GetNodeTemplates_Implementation(UObject* Outer) const
{
	if (UXD_CampRelationshipGraph* CampRelationshipGraph = Cast<UXD_CampRelationshipGraph>(Outer))
	{
		UStringTable* CampNameStringTable = CampRelationshipGraph->CampNameStringTable;
		TArray<FGenericGraph_NewNodeParamers> Res;

		TArray<FString> StringTableKeys;
		CampNameStringTable->GetStringTable()->EnumerateSourceStrings([&](const FString& Key, const FString& SourceString)
		{
			if (!CampRelationshipGraph->CampList.ContainsByPredicate([&](UXD_CampInfo* Camp) {return Camp->CampName.ToString() == SourceString; }))
			{
				StringTableKeys.Add(Key);
			}
			return true;
		});

		for (const FString& Key : StringTableKeys)
		{
			UXD_CampRelationshipGraph_Node* Template = NewObject<UXD_CampRelationshipGraph_Node>(Outer);
			Template->CampInfo = NewObject<UXD_CampInfo>(Outer, GetDefault<UXD_CampSystemSetting>()->CampInfoClass);
			if (UStringTable* CampNameStringTable = GetDefault<UXD_CampSystemSetting>()->GetCampNameStringTable())
			{
				Template->CampInfo->CampName = FText::FromStringTable(CampNameStringTable->GetStringTableId(), Key);
				Template->CampInfo->CampGuid = FGuid::NewGuid();
			}
			FGenericGraph_NewNodeParamers GenericGraph_NewNodeParamers;
			GenericGraph_NewNodeParamers.NodeTemplate = Template;
			GenericGraph_NewNodeParamers.InNodeCategory = LOCTEXT("创建阵营", "创建阵营");
			GenericGraph_NewNodeParamers.InMenuDesc = Template->CampInfo->CampName;
			Res.Add(GenericGraph_NewNodeParamers);
		}

		return Res;
	}
	else
	{
		return {};
	}
}

TArray<FGenericGraph_NewEdgeParamers> UXD_CampRelationshipGraph_Node::GetEdgeTemplates_Implementation(UObject* Outer) const
{
	UXD_CampRelationshipGraph_Edge* Template = NewObject<UXD_CampRelationshipGraph_Edge>(Outer);
	Template->CampRelationship = NewObject<UXD_CampRelationship>(Outer, GetDefault<UXD_CampSystemSetting>()->CampRelationshipClass);
	FGenericGraph_NewEdgeParamers GenericGraph_NewEdgeParamers;
	GenericGraph_NewEdgeParamers.EdgeTemplate = Template;
	GenericGraph_NewEdgeParamers.InNodeCategory = LOCTEXT("阵营", "阵营");
	GenericGraph_NewEdgeParamers.InMenuDesc = LOCTEXT("创建阵营关系", "创建阵营关系");
	return { GenericGraph_NewEdgeParamers };
}

void UXD_CampRelationshipGraph_Node::CheckNodeError_Implementation(EGenericGraphNodeCheckMessageType& GenericGraphNodeCheckMessageType, FString& Message, const FGenericGraphNodeErrorCollector& ErrorCollector) const
{
#if WITH_EDITOR
	if (CampInfo)
	{
		if (!CampInfo->CampName.IsFromStringTable())
		{
			GenericGraphNodeCheckMessageType = EGenericGraphNodeCheckMessageType::Error;
			Message = TEXT("阵营名必须使用StringTable的值");
			return;
		}

		if (UXD_CampRelationshipGraph* CampRelationshipGraph = Cast<UXD_CampRelationshipGraph>(GetGraph()))
		{
			for (UGenericGraphNode* Node : CampRelationshipGraph->AllNodes_Editor)
			{
				if (UXD_CampRelationshipGraph_Node* CampNode = Cast<UXD_CampRelationshipGraph_Node>(Node))
				{
					if (this != CampNode && CampNode->CampInfo)
					{
						if (CampInfo->CampName.EqualTo(CampNode->CampInfo->CampName))
						{
							GenericGraphNodeCheckMessageType = EGenericGraphNodeCheckMessageType::Error;
							Message = FString::Printf(TEXT("已存在阵营[%s]，不得重复"), *CampInfo->CampName.ToString());
							return;
						}
					}
				}
			}
		}
	}
#endif
}

bool UXD_CampRelationshipGraph_Node::CanCreateConnectionWithOtherNode_Implementation(UGenericGraphNode* Other, FText& ErrorMessage) const
{
	if (UXD_CampRelationshipGraph_Edge* CampEdge = Cast<UXD_CampRelationshipGraph_Edge>(Other->GetEdge(this)))
	{
		if (CampEdge->bRelationshipTwoWay)
		{
			ErrorMessage = LOCTEXT("存在启用同等关系节点，不得再创建连接", "存在启用同等关系节点，不得再创建连接");
			return false;
		}
	}
	return true;
}

FText UXD_CampRelationshipGraph_Edge::GetDescription_Implementation() const
{
	UXD_CampRelationshipGraph_Node* StartCampNode = Cast<UXD_CampRelationshipGraph_Node>(StartNode);
	UXD_CampRelationshipGraph_Node* EndCampNode = Cast<UXD_CampRelationshipGraph_Node>(EndNode);
	if (StartCampNode && StartCampNode->CampInfo && EndCampNode && EndCampNode->CampInfo)
	{
		FText TitleDesc = FText::Format(bRelationshipTwoWay ? LOCTEXT("阵营关系连线描述_双向", "{0} 与 {1}") : LOCTEXT("阵营关系连线描述_单向", "{0} 对 {1}"), StartCampNode->CampInfo->CampName, EndCampNode->CampInfo->CampName);
		return FText::Format(LOCTEXT("CampRelationshipGraph_EdgeDesc", "{0}\n{1}"), TitleDesc, FText::FromString(UXD_ObjectFunctionLibrary::GetObjectPropertysDesc(CampRelationship, UObject::StaticClass())));
	}
	return Super::GetNodeTitle_Implementation();
}

void UXD_CampRelationshipGraph_Edge::WhenGraphRebuild_Implementation()
{
	if (UXD_CampRelationshipGraph_Node* Node = Cast<UXD_CampRelationshipGraph_Node>(EndNode))
	{
		if (CampRelationship && Node->CampInfo)
		{
			CampRelationship->ToCamp = Node->CampInfo;
		}
	}
}

FLinearColor UXD_CampRelationshipGraph_Edge::GetConnectionColor_Implementation() const
{
	if (CampRelationship)
	{
		float Weight = CampRelationship->RelationshipValue / 100.f;
		if (Weight > 0)
		{
			return FMath::Lerp(FLinearColor::White, FLinearColor::Green, Weight);
		}
		else
		{
			return FMath::Lerp(FLinearColor::White, FLinearColor::Red, -Weight);
		}
	}
	return Super::GetConnectionColor_Implementation();
}

void UXD_CampRelationshipGraph_Edge::CheckNodeError_Implementation(EGenericGraphNodeCheckMessageType& GenericGraphNodeCheckMessageType, FString& Message, const FGenericGraphNodeErrorCollector& ErrorCollector) const
{
	if (bRelationshipTwoWay)
	{
		if (UXD_CampRelationshipGraph_Edge* CampEdge = Cast<UXD_CampRelationshipGraph_Edge>(EndNode->GetEdge(StartNode)))
		{
			GenericGraphNodeCheckMessageType = EGenericGraphNodeCheckMessageType::Error;
			Message = TEXT("已存在独立的阵营关系，不得开启同等关系");
			return;
		}
	}
}

#undef LOCTEXT_NAMESPACE
