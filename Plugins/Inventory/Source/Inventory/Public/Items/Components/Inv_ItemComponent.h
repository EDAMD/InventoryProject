// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"

/**
 * 可拾取物品拥有的组件, 含有拾取信息等变量
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FInv_ItemManifest GetItemManifest() { return ItemManifest; }
	FString GetPickupMessage() { return PickupMessage; }

	void Pickedup();	// 拾取后调用, 销毁物品

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedup(); // Blueprint 中实现 拾取后调用

protected:

private:
	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInv_ItemManifest ItemManifest;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
		
};
