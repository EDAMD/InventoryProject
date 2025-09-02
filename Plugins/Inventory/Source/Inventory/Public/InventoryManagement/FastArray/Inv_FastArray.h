// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Inv_FastArray.generated.h"

class UInv_InventoryItem;
class UInv_InventoryComponent;
struct FInv_InventoryArray;

/**
 * A single Entry in an Inventory
 */
USTRUCT(BlueprintType)
struct FInv_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInv_InventoryEntry();

private:
	friend FInv_InventoryArray;
	friend UInv_InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInv_InventoryItem> Item = nullptr;
};

/*
 * List of inventory items 
 */
USTRUCT(BlueprintType)
struct FInv_InventoryArray : public FFastArraySerializer
{
	GENERATED_BODY()
	FInv_InventoryArray() : OwnerComponent(nullptr) {}
	FInv_InventoryArray(UActorComponent* InOwner) : OwnerComponent(InOwner) {}

	TArray<UInv_InventoryItem*> GetAllItems() const;

	/* FFastArraySerializer Contract */
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	/* End FFastArraySerializer Contract */

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInv_InventoryEntry, FInv_InventoryArray>(Entries, DeltaParms, *this);
	}

	UInv_InventoryItem* AddItem(UInv_InventoryComponent* ItemComponent);
	UInv_InventoryItem* AddItem(UInv_InventoryItem* Item);
	void RemoveItem(UInv_InventoryItem* Item);

private:
	friend UInv_InventoryComponent;

	// Repicated list of items
	UPROPERTY()
	TArray<FInv_InventoryEntry> Entries;


	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInv_InventoryArray> : public TStructOpsTypeTraitsBase2<FInv_InventoryArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};