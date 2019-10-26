// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakObjectPtr.h"
#include "XD_CampEditor_ClassHelper.generated.h"

/**
 *
 */

USTRUCT()
struct FXD_CampEditor_ClassData
{
	GENERATED_USTRUCT_BODY()

	FXD_CampEditor_ClassData() {}
	FXD_CampEditor_ClassData(UClass* InClass, const FString& InDeprecatedMessage);
	FXD_CampEditor_ClassData(const FString& InAssetName, const FString& InGeneratedClassPackage, const FString& InClassName, UClass* InClass);

	FString ToString() const;
	FString GetClassName() const;
	FText GetCategory() const;
	FString GetDisplayName() const;
	UClass* GetClass(bool bSilent = false);
	bool IsAbstract() const;

	FORCEINLINE bool IsBlueprint() const { return AssetName.Len() > 0; }
	FORCEINLINE bool IsDeprecated() const { return DeprecatedMessage.Len() > 0; }
	FORCEINLINE FString GetDeprecatedMessage() const { return DeprecatedMessage; }
	FORCEINLINE FString GetPackageName() const { return GeneratedClassPackage; }

	/** set when child class masked this one out (e.g. always use game specific class instead of engine one) */
	uint32 bIsHidden : 1;

	/** set when class wants to hide parent class from selection (just one class up hierarchy) */
	uint32 bHideParent : 1;

private:

	/** pointer to uclass */
	TWeakObjectPtr<UClass> Class;

	/** path to class if it's not loaded yet */
	UPROPERTY()
	FString AssetName;

	UPROPERTY()
	FString GeneratedClassPackage;

	/** resolved name of class from asset data */
	UPROPERTY()
	FString ClassName;

	/** User-defined category for this class */
	UPROPERTY()
	FText Category;

	/** message for deprecated class */
	FString DeprecatedMessage;
};

struct FXD_CampEditor_ClassNode
{
	FXD_CampEditor_ClassData Data;
	FString ParentClassName;

	TSharedPtr<FXD_CampEditor_ClassNode> ParentNode;
	TArray<TSharedPtr<FXD_CampEditor_ClassNode> > SubNodes;

	void AddUniqueSubNode(TSharedPtr<FXD_CampEditor_ClassNode> SubNode);
};

struct FXD_CampEditor_ClassHelper
{
public:
	DECLARE_MULTICAST_DELEGATE(FOnPackageListUpdated);

	FXD_CampEditor_ClassHelper(UClass* InRootClass);
	~FXD_CampEditor_ClassHelper();

	void GatherClasses(const UClass* BaseClass, TArray<FXD_CampEditor_ClassData>& AvailableClasses);
	static FString GetDeprecationMessage(const UClass* Class);

	void OnAssetAdded(const struct FAssetData& AssetData);
	void OnAssetRemoved(const struct FAssetData& AssetData);
	void InvalidateCache();
	void OnHotReload(bool bWasTriggeredAutomatically);

	static void AddUnknownClass(const FXD_CampEditor_ClassData& ClassData);
	static bool IsClassKnown(const FXD_CampEditor_ClassData& ClassData);
	static FOnPackageListUpdated OnPackageListUpdated;

	static int32 GetObservedBlueprintClassCount(UClass* BaseNativeClass);
	static void AddObservedBlueprintClasses(UClass* BaseNativeClass);
	void UpdateAvailableBlueprintClasses();

private:

	UClass* RootNodeClass;
	TSharedPtr<FXD_CampEditor_ClassNode> RootNode;
	static TArray<FName> UnknownPackages;
	static TMap<UClass*, int32> BlueprintClassCount;

	TSharedPtr<FXD_CampEditor_ClassNode> CreateClassDataNode(const struct FAssetData& AssetData);
	TSharedPtr<FXD_CampEditor_ClassNode> FindBaseClassNode(TSharedPtr<FXD_CampEditor_ClassNode> Node, const FString& ClassName);
	void FindAllSubClasses(TSharedPtr<FXD_CampEditor_ClassNode> Node, TArray<FXD_CampEditor_ClassData>& AvailableClasses);

	UClass* FindAssetClass(const FString& GeneratedClassPackage, const FString& AssetName);
	void BuildClassGraph();
	void AddClassGraphChildren(TSharedPtr<FXD_CampEditor_ClassNode> Node, TArray<TSharedPtr<FXD_CampEditor_ClassNode> >& NodeList);

	bool IsHidingClass(UClass* Class);
	bool IsHidingParentClass(UClass* Class);
	bool IsPackageSaved(FName PackageName);
};