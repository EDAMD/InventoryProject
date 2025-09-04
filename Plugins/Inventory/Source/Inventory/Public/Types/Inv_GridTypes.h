#pragma once

#include "CoreMinimal.h"
#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None,
};

USTRUCT(BlueprintType)
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability() {}
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	int32 Index{ INDEX_NONE };				// 物品被放入网格的索引
	int32 AmountToFill{ 0 };				// 填充数量
	bool bItemAtIndex{ false };				// 当前物品是否存在于网格中, 并且含有 Index
};

USTRUCT(BlueprintType)
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	TWeakObjectPtr<UInv_InventoryItem> Item;			// 物品信息
	int32 TotalRoomToFill{ 0 };							// 需要占用格子数
	int32 Remainder{ 0 };								// 剩余数量
	bool bStackable{ false };							// 是否可堆叠
	TArray<FInv_SlotAvailability> SlotAvailabilities;	// 占用网格信息
};