// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemManifest.generated.h"

class UInv_InventoryItem;

/**
 * The item Manifest contains all off the necessary data
 * for creating a new Invenotry Item
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	UInv_InventoryItem* Manifest(UObject* Outer);
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{ EInv_ItemCategory::None };
};
