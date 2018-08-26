// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampRelationship.h"
#include <UnrealNetwork.h>




UXD_CampRelationship::UXD_CampRelationship()
{

}

void UXD_CampRelationship::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
	DOREPLIFETIME(UXD_CampRelationship, RelationshipValue);
}
