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

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& ItemComponent);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	UFUNCTION()
	void RemoveItem(UInv_InventoryItem* Item);
	
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
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 TileSize;

	bool MatchesCategory(const UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(UInv_InventoryItem* Item);
};
