// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemManifest.generated.h"

class UInv_InventoryItem;

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
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{ EInv_ItemCategory::None };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;
};
