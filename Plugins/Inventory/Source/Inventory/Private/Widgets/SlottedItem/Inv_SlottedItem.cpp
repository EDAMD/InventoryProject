// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SlottedItem/Inv_SlottedItem.h"
#include "Components/Image.h"
#include "Items/Inv_InventoryItem.h"



void UInv_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UInv_SlottedItem::SetInventoryItem(UInv_InventoryItem* Item)
{
	InventoryItem = Item;
}