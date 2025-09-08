#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemFragment.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()

	FInv_ItemFragment() {}
	FInv_ItemFragment(const FInv_ItemFragment&) = default;					// 拷贝构造函数
	FInv_ItemFragment& operator=(const FInv_ItemFragment&) = default;		// 拷贝赋值运算符
	FInv_ItemFragment(FInv_ItemFragment&&) = default;						// 移动构造函数
	FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default;			// 移动赋值运算符
	virtual ~FInv_ItemFragment() {}											// 析构函数

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{ 1, 1 };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{ 0.f };
};

USTRUCT(BlueprintType)
struct FInv_ImageFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

public:
	UTexture2D* GetIcon() const { return Icon; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimensions{44.f, 44.f};
};

USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};		// 栈存放最大值 (一个格子可存放数量)
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{1};		// 捡起时获得数量
};