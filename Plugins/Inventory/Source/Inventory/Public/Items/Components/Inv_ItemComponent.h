// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	FORCEINLINE FString GetPickupMessage() { return PickupMessage; }

protected:

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
		
};
