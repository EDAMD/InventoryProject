// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Items/Inv_InventoryItem.h"


FInv_InventoryEntry::FInv_InventoryEntry()
{

}

TArray<UInv_InventoryItem> FInv_InventoryArray::GetAllItems() const
{
	return TArray<UInv_InventoryItem>();
}

void FInv_InventoryArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{

}

void FInv_InventoryArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{

}

UInv_InventoryItem* FInv_InventoryArray::AddItem(UInv_InventoryComponent* ItemComponent)
{
	return nullptr;
}

UInv_InventoryItem* FInv_InventoryArray::AddItem(UInv_InventoryItem* Item)
{
	return nullptr;
}

void FInv_InventoryArray::RemoveItem(UInv_InventoryItem* Item)
{

}

