// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_InventoryComponent;
class UInv_ItemComponent;


/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComp(UInv_ItemComponent* Comp);

	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function);
};

template<typename T, typename FuncT>
void UInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function)
{
	for (int j = 0; j < Range2D.Y; j++)
	{
		for (int i = 0; i < Range2D.X; i++)
		{
			Array[Index + i + j * GridColumns];

			// 获得含有循环变量的 二维点
			const FIntPoint Coordinates = UInv_WidgetUtils::GetPositionFromIndex(Index, GridColumns) + FIntPoint(i, j);
			// 将二维中的点转换为数字中一维的点
			const int32 TileIndex = UInv_WidgetUtils::GetIndexFromPosition(Coordinates, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
			}
		}
	}
}
