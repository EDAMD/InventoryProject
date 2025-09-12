// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/GridSlots/Inv_GridSlot.h"
#include "Components/CanvasPanel.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Widgets/SlottedItem/Inv_SlottedItem.h"
#include "Styling/SlateBrush.h"
#include "GameplayTagContainer.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Inventory.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &UInv_InventoryGrid::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &UInv_InventoryGrid::AddStacks);
}

void UInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPosition = UInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePositon = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	UpdateTileParameters(CanvasPosition, MousePositon);
}

void UInv_InventoryGrid::UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	// if mouse not in canvas panel. return

	// Calculate the tile quadrant, tile index and coordinates
	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	LastTileParameters = TileParameters;

	TileParameters.TileCoordinates = HoveredTileCoordinates;
	TileParameters.TileIndex = UInv_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameters.TileQuadrant = CalculateQuadrant(CanvasPosition, MousePosition);

	// Handle highlight/unhighlight of the grid slots
	OnTileParametersUpdated(TileParameters);
}

void UInv_InventoryGrid::OnTileParametersUpdated(const FInv_TileParameters& Parameters)
{
	if (!IsValid(HoverItem)) return;

	// Get hover item's dimensions
	const FIntPoint Dimensions = HoverItem->GetGridDimensions();
	
	// Calculate the starting coordinate for highlighting
	const FIntPoint StartingCoordinate = CalculateStartingCoordinates(Parameters.TileCoordinates, Dimensions, Parameters.TileQuadrant);

	// check hover position
		// in the grid bounds
		// any item in the way
		// if so, is there only one item in the way. (can we swap?)
		// 
}

FIntPoint UInv_InventoryGrid::CalculateStartingCoordinates(const FIntPoint& Coordinate, const FIntPoint Dimensions, const EInv_TileQuadrant Quadrant) const
{
	// ���� �ߴ����ż�������ж� StartingCoordinate ---ż��, ����������ڸ��������ж�. ---����, ����������ڸ���λ���ж�
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEventHight = Dimensions.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoord;
	switch (Quadrant)
	{
		case EInv_TileQuadrant::TopLeft:
		{
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
			break;
		}
		case EInv_TileQuadrant::TopRight:
		{
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
			break;
		}
		case EInv_TileQuadrant::BottomLeft:
		{
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEventHight;
			break;
		}
		case EInv_TileQuadrant::BottomRight:
		{
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEventHight;
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEventHight;
			break;
		}
		default:
			UE_LOG(LogInventory, Error, TEXT("Invalid Quadrant."))
			return FIntPoint(-1, -1);
	}
	return StartingCoord;
}

FIntPoint UInv_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	return FIntPoint{
		FMath::FloorToInt32((MousePosition.X - CanvasPosition.X) / TileSize),
		FMath::FloorToInt32((MousePosition.Y - CanvasPosition.Y) / TileSize)
	};
}

EInv_TileQuadrant UInv_InventoryGrid::CalculateQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	// Calculate relative position within current tile 
	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, TileSize);
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, TileSize);

	// Determined which quadrant the mouse is in
	const bool bIsTop = TileLocalY < TileSize / 2.f;
	const bool bIsLeft = TileLocalX < TileSize / 2.f;

	EInv_TileQuadrant HoveredTileQuadrant{EInv_TileQuadrant::None};
	if (bIsTop && bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = EInv_TileQuadrant::BottomRight;

	return HoveredTileQuadrant;
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	// Create a widget to show item icon and add it to the correct spot on the grid.
	AddItemToIndices(Result, Item);
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* Item)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		// ���� SlotItem ��Ϣ
		AddItemAtIndex(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);

		// ���� GridSlot ��Ϣ
		UpdateGridSlot(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}

}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
	FInv_SlotAvailabilityResult Result;

	// 1. ȷ��Item�Ƿ��� Stackable
	const FInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	// 2. ȷ����ջ����ӵ�����
	const int32 MaxStackSize = Result.bStackable ? StackableFragment->GetMaxStackSize() : 1; // ����Ʒ���ѵ�����
	int32 AmountToFill = Result.bStackable ? StackableFragment->GetStackCount() : 1;		 // ����Ʒ�ѵ�������

	TSet<int32> CheckedIndices;	// �Ѿ���������Ԫ���±�

	// 3. ����ÿһ�� GridSlot:
	for (const TObjectPtr<UInv_GridSlot>& GridSlot : GridSlots)
	{
		// ���û��ʣ����Ҫ���Ķ���, ��ǰ�˳�ѭ��
		if (AmountToFill == 0) break;

		// �ж���� ���� ���ڵĸ����Ƿ�ռ��
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue;

		// ��������Ʒ�Ƿ񳬳��������ӵı߽�(��, 2x3�Ķ���, ֻ���� 1, 2�з���, �ڶ����Ժ�ͳ����߽���)
		if (!IsInGridBounds(GridSlot->GetIndex(), GetItemDimensions(Manifest))) continue;

		// �ж���������Ƿ���ȫ�ŵ���(��, 3 x 2 �ĸ��Ӳ��ܷ��� 3 x 3 ����Ʒ) (i.e. Is it out of grid bounds?)
		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativelyClaimed, Manifest.GetItemType(), MaxStackSize))
		{
			continue;
		}

		// ����������
		const int32 AmountToFillInSlot = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;

		// ��(HasRoomAtIndex -> true ʱ, ���ݴ�ı�����������ӵ� CheckedIndices ��)
		CheckedIndices.Append(TentativelyClaimed);

		// ���¸���ʣ����������
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FInv_SlotAvailability{
			HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetIndex(),
			Result.bStackable ? AmountToFillInSlot : 0,
			HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillInSlot;

		// 4. ʰȡ��ʣ������
		Result.Remainder = AmountToFill;

		if (AmountToFill == 0) return Result;
	}


	return Result;
}

bool UInv_InventoryGrid::HasRoomAtIndex(
	UInv_GridSlot* GridSlot,
	FIntPoint Dimensions,
	const TSet<int32>& CheckedIndices,
	TSet<int32>& OutTentativelyClaimed,
	const FGameplayTag& ItemType,
	const int32 MaxStackSize)
{
	// ���������Ӧ�ĸ����Ƿ���������Ʒ�赲(�� ��һ�� 3 x 3 �ĸ�����, �м����������Ʒռ����һ������) (i.e. are there other item in the way?)
	bool bHasRoomAtIndex = true;

	UInv_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns,
		[&](const UInv_GridSlot* SubGridSlot)
		{
			// ���������Ҫ����
			if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices, OutTentativelyClaimed, ItemType, MaxStackSize))
			{
				OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
			}
			else
			{
				bHasRoomAtIndex = false;
			}
		}
	);

	return bHasRoomAtIndex;
}

bool UInv_InventoryGrid::CheckSlotConstraints(
	const UInv_GridSlot* GridSlot,
	const UInv_GridSlot* SubGridSlot,
	const TSet<int>& CheckedIndices,
	TSet<int32>& OutTentativelyClaimed,
	const FGameplayTag& ItemType,
	const int32 MaxStackSize) const
{
	// ��� �����Ƿ�ռ��
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetIndex())) return false;

	// ��������Ƿ���Ч�� Item û����������ǿյ�
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		return true;
	}

	// Դ�����Ƿ������������������(UpperLeftSlot) --- ����Ʒ�ǿɶѵ���Ʒ, ������Ҫ�ж� Դ���� �Ƿ��������� Ч�������� ����������
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false;

	// �ж��Ƿ��� Stackable item
	const UInv_InventoryItem* Item = SubGridSlot->GetInventoryItem().Get();
	if (!Item->IsStackable()) return false;

	// ������õĸ����Ƿ���������Ҫ��ӵ���Ʒ����һ��
	if (!DoesItemTypeMatch(Item, ItemType)) return false;

	// Is Stackable, �ж����Ƿ�ﵽ�������
	if (GridSlot->GetStackCount() >= MaxStackSize) return false;

	return true;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
	const FInv_GridFragment* GridFragment = Manifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UInv_InventoryGrid::HasValidItem(const UInv_GridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGrid::IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool UInv_InventoryGrid::DoesItemTypeMatch(const UInv_InventoryItem* Item, const FGameplayTag& ItemType) const
{
	return Item->GetItemManifest().GetItemType() == ItemType;
}

bool UInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;

	int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;

	return EndRow <= Rows && EndColumn <= Columns;
}

int32 UInv_InventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlot* GridSlot) const
{
	// �������ʣ������
	int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);

	// ����ǿɶѵ���Ʒ, ��Ҫ AmountToFill �� ����ʣ����������Сֵ
	return bStackable ? FMath::Min(RoomInSlot, AmountToFill) : 1;
}

int32 UInv_InventoryGrid::GetStackAmount(const UInv_GridSlot* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();

	// ����������û��StackCount, ��ô���Ǿ���Ҫ��ȡ�������ĸ��ӵ� StackCount
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UInv_GridSlot* UpperLeftSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftSlot->GetStackCount();
	}
	return CurrentSlotStackCount;
}


void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount)
{
	// ��ȡ GridFragment, ���ǾͿ���֪����� ��Ʒ ռ�ö��� Grid
	// ��ȡ ImageFragment, ���ǾͿ��� ����������ʾ Icom
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::ImageFragment);
	if (!GridFragment || !ImageFragment) return;

	// ����һ�� Widget ��ӵ� Grid ��
	UInv_SlottedItem* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);

	// �� SlottedItem ��ӵ� Canvas Pannel ��
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	// �������д洢�´����� Widget .
	SlottedItems.Add(Index, SlottedItem);
}

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(
	UInv_InventoryItem* Item,
	const bool bStackable,
	const int32 StackAmount,
	const FInv_GridFragment* GridFragment,
	const FInv_ImageFragment* ImageFragment,
	const int32 Index)
{
	UInv_SlottedItem* SlottedItem = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::OnSlottedItemClicked);
	return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem)
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

void UInv_InventoryGrid::UpdateGridSlot(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	// �������Ͻǵĸ��Ӽ�¼����
	if (bStackableItem)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FInv_GridFragment* GridFramgent = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FIntPoint Dimensions = GridFramgent ? GridFramgent->GetGridSize() : FIntPoint(1, 1);

	/* ����������(GridSlot) ������Ʒ�ߴ����Ӧ�ĸ��ӱ���
	for (int j = 0; j < GridFramgent->GetGridSize().Y; j++)
	{
		for (int i = 0; i < GridFramgent->GetGridSize().X; i++)
		{
			UInv_GridSlot* GridSlot = GridSlots[Index + Columns* j + i];
			GridSlot->SetOccupiedTexture();
		}
	}*/

	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns,
		[&](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(NewItem);	// ����ӵ����Ʒ
			GridSlot->SetUpperLeftIndex(Index);		// �������ĸ���
			GridSlot->SetOccupiedTexture();			// ���ø��ӱ���
			GridSlot->SetAvailable(false);			// ���ø���״̬
		}
	);
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UInv_InventoryGrid::SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	FVector2D IconSize = GetDrawSize(GridFragment);
	Brush.ImageSize = IconSize;
	SlottedItem->SetImageBrush(Brush);
}

void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int j = 0; j < Rows; j++)
	{
		for (int i = 0; i < Columns; i++)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);

			CanvasPanel->AddChild(GridSlot);

			FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

void UInv_InventoryGrid::AddStacks(const FInv_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const FInv_SlotAvailability& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)	// ��� Availability �Ǻ�����ͬ���͵� GridSlot �Ҷ�ջ��ʣ��������, �ʲ����½� SlotItem
		{
			const auto& GridSlot = GridSlots[Availability.Index];
			const auto& SlotItem = SlottedItems[Availability.Index];
			SlotItem->UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
		}
		else // ��� Availability ����������֮ǰ GridSlot ��ջ���������, �½��� GridSlot, ���½�һ�� SlotItem
		{
			AddItemAtIndex(Result.Item.Get(), Availability.Index, Result.bStackable, Result.TotalRoomToFill);
			UpdateGridSlot(Result.Item.Get(), Availability.Index, Result.bStackable, Result.TotalRoomToFill);
		}
	}

}

void UInv_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UInv_InventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	if (!IsValid(HoverItem) && IsLeftClicked(MouseEvent))
	{
		// Pickup --- Assign the hover item and remove slotted item from the grid
		Pickup(ClickedInventoryItem, GridIndex);
	}
}

bool UInv_InventoryGrid::IsRightClicked(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UInv_InventoryGrid::IsLeftClicked(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UInv_InventoryGrid::Pickup(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	// Assign the hover item
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	// remove clicked item from the grid
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	
}

void UInv_InventoryGrid::AssignHoverItem(UInv_InventoryItem* Item, const int32 GridIndex, const int32 PrevoidGridIndex)
{
	AssignHoverItem(Item);

	HoverItem->SetPreviousGridIndex(PrevoidGridIndex);
	HoverItem->UpdateStackCount(Item->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UInv_InventoryGrid::AssignHoverItem(UInv_InventoryItem* Item)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<UInv_HoverItem>(GetOwningPlayer(), HoverItemClass);
	}
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::ImageFragment);
	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);

	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(Item);
	HoverItem->SetStackable(Item->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);

}

void UInv_InventoryGrid::RemoveItemFromGrid(UInv_InventoryItem* Item, const int32 GridIndex)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return;

	UInv_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns,
		[](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetUpperLeftIndex(INDEX_NONE);
			GridSlot->SetUnoccupiedTexture();
			GridSlot->SetAvailable(true);
			GridSlot->SetStackCount(0);
		}
	);

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<UInv_SlottedItem> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}

}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item)
{
	return ItemCategory == Item->GetItemManifest().GetItemCategory();
}
