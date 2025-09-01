// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Inv_Highlightable.h"
#include "Inv_HighlightableStaticMesh.generated.h"

/**
 * 继承自 UStaticMeshComponent, 实现了 Inv_Highlightable 接口的组件, 实现了 StaticMesh 的 高亮 与 取消高亮 功能
 */
UCLASS()
class INVENTORY_API UInv_HighlightableStaticMesh : public UStaticMeshComponent, public IInv_Highlightable
{
	GENERATED_BODY()
	
public:
	virtual void Highlight_Implementation() override;

	virtual void UnHighlight_Implementation() override;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
