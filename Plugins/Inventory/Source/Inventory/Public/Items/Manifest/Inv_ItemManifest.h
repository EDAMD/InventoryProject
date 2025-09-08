// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemManifest.generated.h"

class UInv_InventoryItem;
struct FInv_ItemFragment;

/**
 * item Manifest 包含所有需要的 数据(创建一个新的 InventoryItem 所需要的数据)
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	// 创建 InventoryItem 副本
	UInv_InventoryItem* Manifest(UObject* Outer); 
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfType() const;

private:							
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct)) // ExcludeBaseStruct 防止添加基类
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{ EInv_ItemCategory::None };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;
};

template<typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;

			return FragmentPtr;
		}
	}

	return nullptr;
}

template<typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fagments)
	{
		if (const T* FragmentPtr = Frament.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}
