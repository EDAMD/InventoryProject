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
	int32 AmountToFill{ 0 };				// 物品被放入网格的数量
	bool bItemAtIndex{ false };				// 当前物品是否存在于网格中, 并且含有 Index
};

USTRUCT(BlueprintType)
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	TWeakObjectPtr<UInv_InventoryItem> Item;			// 物品信息
	int32 TotalRoomToFill{ 0 };							// 拾取物品的总数
	int32 Remainder{ 0 };								// 剩余数量
	bool bStackable{ false };							// 是否可堆叠
	TArray<FInv_SlotAvailability> SlotAvailabilities;	// 占用网格信息
};

UENUM(BlueprintType)
enum class EInv_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None,
};

USTRUCT(BlueprintType)
struct FInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FIntPoint TileCoordinates{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
};

inline bool operator==(const FInv_TileParameters& A, const FInv_TileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates && A.TileIndex == B.TileIndex && A.TileQuadrant == B.TileQuadrant;
}

USTRUCT()
struct FInv_SpaceQueryResult
{
	GENERATED_BODY()

	// True if the space queried has no item in it;
	bool bHasSpace{ false };

	// Valid if there is a single item we can swap with
	TWeakObjectPtr<UInv_InventoryItem> ValidItem = nullptr;

	// Upper left index of the valid item, if there is one
	int32 UpperLeftIndex{ INDEX_NONE };
};