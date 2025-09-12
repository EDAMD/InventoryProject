// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"

class UInv_GridSlot;
class UCanvasPanel;
class UInv_InventoryComponent;
class UInv_ItemComponent;
struct FInv_ItemManifest;
class UInv_SlottedItem;
struct FInv_GridFragment;
struct FInv_ImageFragment;
struct FGameplayTag;
class UInv_HoverItem;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);
	
private:

	void ConstructGrid();

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EInv_ItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItem>> SlottedItems;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem;

	/*  */
	FInv_TileParameters TileParameters;
	FInv_TileParameters LastTileParameters;
	int32 ItemDropIndex{ INDEX_NONE };		// Index where an item will be placed if we click on the grid at a valid location
	FInv_SpaceQueryResult CurrentQueryResult;

	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EInv_TileQuadrant CalculateQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersUpdated(const FInv_TileParameters& Parameters);
	FIntPoint CalculateStartingCoordinates(const FIntPoint& Coordinate, const FIntPoint Dimensions, const EInv_TileQuadrant Quadrant) const;
	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);
	/* End */

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 TileSize;

	/*  */
	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
	bool IsRightClicked(const FPointerEvent& MouseEvent) const;
	bool IsLeftClicked(const FPointerEvent& MouseEvent) const;
	void Pickup(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	void AssignHoverItem(UInv_InventoryItem* Item);
	void AssignHoverItem(UInv_InventoryItem* Item, const int32 GridIndex, const int32 PrevoidGridIndex);
	void RemoveItemFromGrid(UInv_InventoryItem* Item, const int32 GridIndex);
	/* End */


	/*  */
	bool MatchesCategory(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item);
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	UInv_SlottedItem* CreateSlottedItem(
		UInv_InventoryItem* Item, 
		const bool bStackable, 
		const int32 StackAmount, 
		const FInv_GridFragment* GridFragment, 
		const FInv_ImageFragment* ImageFragment, 
		const int32 Index);
	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem);
	void UpdateGridSlot(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount);
	bool IsIndexClaimed(const TSet<int>& CheckedIndices, const int32 Index) const;
	bool HasRoomAtIndex(
		UInv_GridSlot* GridSlot,
		FIntPoint Dimensions, 
		const TSet<int32>& CheckedIndices, 
		TSet<int32>& OutTentativelyClaimed, 
		const FGameplayTag& ItemType,
		const int32 MaxStackSize
	);
	bool CheckSlotConstraints(
		const UInv_GridSlot* GridSlot, 
		const UInv_GridSlot* SubGridSlot, 
		const TSet<int>& CheckedIndices, 
		TSet<int32>& OutTentativelyClaimed, 
		const FGameplayTag& ItemType,
		const int32 MaxStackSize
	) const;
	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const;
	bool HasValidItem(const UInv_GridSlot* GridSlot) const;
	bool IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const;
	bool DoesItemTypeMatch(const UInv_InventoryItem* Item, const FGameplayTag& ItemType) const;
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlot* GridSlot) const;
	int32 GetStackAmount(const UInv_GridSlot* GridSlot) const;
	/* End */


};
