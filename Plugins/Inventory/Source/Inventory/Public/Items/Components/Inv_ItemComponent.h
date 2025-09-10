// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"

/**
 * ��ʰȡ��Ʒӵ�е����, ����ʰȡ��Ϣ�ȱ���
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

	void Pickedup();	// ʰȡ�����, ������Ʒ

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedup(); // Blueprint ��ʵ�� ʰȡ�����

protected:

private:
	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInv_ItemManifest ItemManifest;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
		
};
