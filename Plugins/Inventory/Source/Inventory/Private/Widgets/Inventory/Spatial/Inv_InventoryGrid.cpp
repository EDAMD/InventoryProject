// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/GridSlots/Inv_GridSlot.h"
#include "Components/CanvasPanel.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &UInv_InventoryGrid::AddItem);
	InventoryComponent->OnItemRemoved.AddDynamic(this, &UInv_InventoryGrid::RemoveItem);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& ItemComponent)
{
	FInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;
	return Result;
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	// TODO: Create a widget to show item icon and add it to the correct spot on the grid.
	
}

void UInv_InventoryGrid::RemoveItem(UInv_InventoryItem* Item)
{

}

void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);

			CanvasPanel->AddChild(GridSlot);

			FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item)
{
	return ItemCategory == Item->GetItemManifest().GetItemCategory();
}
