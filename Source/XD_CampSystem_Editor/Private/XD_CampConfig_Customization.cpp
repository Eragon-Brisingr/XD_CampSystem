// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampConfig_Customization.h"
#include "XD_CampSystemType.h"
#include <Internationalization/StringTable.h>
#include "XD_CampSystemSetting.h"
#include <DetailWidgetRow.h>
#include "XD_CampSystem_EditorUtility.h"
#include <Widgets/Input/STextComboBox.h>
#include <Internationalization/StringTableCore.h>
#include "XD_CampGraph.h"
#include "XD_CampInfo.h"

namespace CampPropertyCustomize
{
	UObject* GetOuter(const TSharedRef<IPropertyHandle>& PropertyHandle)
	{
		TArray<UObject*> Outers;
		PropertyHandle->GetOuterObjects(Outers);
		return Outers.Num() > 0 ? Outers[0] : nullptr;
	}

	template<typename ValueType>
	ValueType* Value(const TSharedPtr<IPropertyHandle>& PropertyHandle)
	{
		if (UObject* Outer = GetOuter(PropertyHandle.ToSharedRef()))
		{
			return reinterpret_cast<ValueType*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(Outer)));
		}
		return nullptr;
	}

	template<typename Type>
	static void SetValue(const TSharedPtr<IPropertyHandle>& PropertyHandle, const Type& Value, bool NotifyChange = true)
	{
		if (NotifyChange)
		{
			PropertyHandle->NotifyPreChange();
		}
		if (Type* Target = reinterpret_cast<Type*>(PropertyHandle->GetValueBaseAddress(reinterpret_cast<uint8*>(GetOuter(PropertyHandle.ToSharedRef())))))
		{
			*Target = Value;
			if (NotifyChange)
			{
				PropertyHandle->NotifyPostChange(EPropertyValueSetFlags::DefaultFlags);
			}
		}
	}
}


void FXD_CampConfig_Customization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> CampName_PropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FXD_CampConfig, CampName));
	TSharedPtr<IPropertyHandle> CampGuid_PropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FXD_CampConfig, CampGuid));

	bool HasValidCampName = false;
	UXD_CampGraph* CampGraph = GetDefault<UXD_CampSystemSetting>()->GlobalCampGraph.LoadSynchronous();
	if (CampGraph)
	{
		CampNameList = CampGraph->GetAllCampNames();

		if (const FGuid* CampHandle = CampPropertyCustomize::Value<FGuid>(CampGuid_PropertyHandle))
		{
			int32 Index = CampGraph->CampList.IndexOfByPredicate([&](const UXD_CampInfo* E) {return E->CampGuid == *CampHandle; });
			if (Index != INDEX_NONE)
			{
				InitSelectedText = CampNameList[Index];
				HasValidCampName = true;
			}
		}
	}

	if (HasValidCampName == false)
	{
		if (const FText* CampName = CampPropertyCustomize::Value<FText>(CampName_PropertyHandle))
		{
			InitSelectedText = MakeShareable(new FString(FString::Printf(TEXT("[%s]_无效的阵营名"), *CampName->ToString())));
		}
	}

	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		[
			SNew(STextComboBox)
			.OptionsSource(&CampNameList)
			.OnSelectionChanged_Lambda([=](TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo)
			{
				if (SelectInfo == ESelectInfo::OnMouseClick)
				{
					UXD_CampGraph* CampGraph = GetDefault<UXD_CampSystemSetting>()->GlobalCampGraph.LoadSynchronous();
					if (UXD_CampInfo** P_CampInfo = CampGraph->CampList.FindByPredicate([&](const UXD_CampInfo* E) {return E->CampName.ToString() == *Selection.Get(); }))
					{
						if (UXD_CampInfo* CampInfo = *P_CampInfo)
						{
							CampName_PropertyHandle->SetValue(CampInfo->CampName);
							CampPropertyCustomize::SetValue(CampGuid_PropertyHandle, CampInfo->CampGuid);
						}
					}
				}
			})
			.InitiallySelectedItem(InitSelectedText)
			.IsEnabled(!StructPropertyHandle->IsEditConst())
		];
}
