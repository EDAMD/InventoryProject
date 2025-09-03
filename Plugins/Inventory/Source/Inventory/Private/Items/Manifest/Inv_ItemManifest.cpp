// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* Outer)
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(Outer, UInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}
