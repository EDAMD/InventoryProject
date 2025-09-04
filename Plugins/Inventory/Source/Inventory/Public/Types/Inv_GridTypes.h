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

	int32 Index{ INDEX_NONE };				// ��Ʒ���������������
	int32 AmountToFill{ 0 };				// �������
	bool bItemAtIndex{ false };				// ��ǰ��Ʒ�Ƿ������������, ���Һ��� Index
};

USTRUCT(BlueprintType)
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	TWeakObjectPtr<UInv_InventoryItem> Item;			// ��Ʒ��Ϣ
	int32 TotalRoomToFill{ 0 };							// ��Ҫռ�ø�����
	int32 Remainder{ 0 };								// ʣ������
	bool bStackable{ false };							// �Ƿ�ɶѵ�
	TArray<FInv_SlotAvailability> SlotAvailabilities;	// ռ��������Ϣ
};