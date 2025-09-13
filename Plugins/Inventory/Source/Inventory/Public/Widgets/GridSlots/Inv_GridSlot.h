// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;
class UInv_InventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, SlotIndex, const FPointerEvent&, MouseEvent);

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut,
};

UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MynGeometry, const FPointerEvent& MouseEvent) override;


	int32 GetTileIndex() const { return TileIndex; }
	void SetTileIndex(int32 Index) { TileIndex = Index; }

	int32 GetIndex() const { return TileIndex; }
	void SetIndex(int32 Index) { TileIndex = Index; }

	EInv_GridSlotState GetGridSlotState() const { return GridSlotState; }

	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UInv_InventoryItem* Item);

	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }

	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }

	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;

private:
	int32 TileIndex{ INDEX_NONE };
	int32 StackCount{0}; // 该格子中物品的数量(仅对可堆叠物品有效)
	int32 UpperLeftIndex{ INDEX_NONE };
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bAvailable{ true };

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GridSlotState;

};
