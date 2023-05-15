// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UUDWindowUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	MousePositionChanged();
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UUDWindowUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsMoving = true;
	FVector2D currentPosition = GetCurrentPosition();
	FVector2D canvasPosition = UWidgetLayoutLibrary::SlotAsCanvasSlot(InnerCanvas)->GetPosition();
	OriginalPosition = currentPosition - canvasPosition;
	return FReply::Handled();
}

FReply UUDWindowUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsMoving = false;
	return FReply::Handled();
}
void UUDWindowUserWidget::MousePositionChanged()
{
	if (IsMoving)
	{
		// Screen bounds.
		FVector2D ltCorner = FVector2D(0, 0);
		FVector2D rbCornerDistance = UWidgetLayoutLibrary::GetViewportSize(GetOwningPlayer());
		// Mouse within screen size.
		FVector2D currentMousePos = GetCurrentPosition();
		// HACK: this effectively prevent's player from leaving screen
		if (currentMousePos.X <= 0.01 && currentMousePos.Y <= 0.01)
		{
			IsMoving = false;
			return;
		}
		
		FVector2D clampedMousePos = currentMousePos;
		// Window within screen size.
		FVector2D newMousePos = clampedMousePos - OriginalPosition;
		FVector2D windowSize = InnerCanvas->GetDesiredSize();

		// TODO: this might be incorrect clamp
		FVector2D clampedWindowPos = FVector2D(
			FMath::Clamp(newMousePos.X, ltCorner.X, rbCornerDistance.X - windowSize.X),
			FMath::Clamp(newMousePos.Y, ltCorner.Y, rbCornerDistance.Y)
		);

		// Set new position.
		UWidgetLayoutLibrary::SlotAsCanvasSlot(InnerCanvas)->SetPosition(clampedWindowPos);
	}
}

FVector2D UUDWindowUserWidget::GetCurrentPosition()
{
	FVector2D currentPosition;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), currentPosition.X, currentPosition.Y);
	return currentPosition;
}