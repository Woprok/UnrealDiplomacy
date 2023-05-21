// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/GridPanel.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

FReply UUDWindowUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == MovementButton)
	{
		UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: StartMovement."));
		return StartMovement(InGeometry, InMouseEvent);
	}
	//if (InMouseEvent.GetEffectingButton() == ResizeButton)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: StartResize."));
	//	return StartResize(InGeometry, InMouseEvent);
	//}
	return FReply::Unhandled();
}

FReply UUDWindowUserWidget::StartMovement(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsMoving = true;
	WindowPosition = UWidgetLayoutLibrary::SlotAsCanvasSlot(WindowSizeBox)->GetPosition();
	PressOffset = InitialMousePosition - WindowPosition;
	FEventReply dragEvent = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, MovementButton);
	FEventReply captureEvent = UWidgetBlueprintLibrary::CaptureMouse(dragEvent, this);
	return captureEvent.NativeReply;
}

FReply UUDWindowUserWidget::StartResize(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsResizing = true;
	FEventReply dragEvent = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, ResizeButton);
	FEventReply captureEvent = UWidgetBlueprintLibrary::CaptureMouse(dragEvent, this);
	return captureEvent.NativeReply;
}

FReply UUDWindowUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply moveReply = FReply::Unhandled();
	if (InMouseEvent.GetEffectingButton() == MovementButton)
	{
		UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: EndMovement."));
		moveReply = EndMovement(InGeometry, InMouseEvent);
	}
	//FReply resizeReply = FReply::Unhandled();
	//if (InMouseEvent.GetEffectingButton() == ResizeButton)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: EndResize."));
	//	resizeReply = EndResize(InGeometry, InMouseEvent);
	//}
	SnapToGrid();
	return moveReply;
}

FReply UUDWindowUserWidget::EndMovement(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsMoving = false;
	FEventReply dragEvent = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, MovementButton);
	FEventReply captureEvent = UWidgetBlueprintLibrary::ReleaseMouseCapture(dragEvent);
	return captureEvent.NativeReply;
}

FReply UUDWindowUserWidget::EndResize(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	IsMoving = false;
	FEventReply dragEvent = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, ResizeButton);
	FEventReply captureEvent = UWidgetBlueprintLibrary::ReleaseMouseCapture(dragEvent);
	return captureEvent.NativeReply;
}

FReply UUDWindowUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	InitialMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	if (IsMoving)
	{
		//UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: OnMovement."));
		return OnMovement(InitialMousePosition);
	}
	//if (IsResizing)
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: OnResize."));
	//	return OnResize(InitialMousePosition);
	//}
	return FReply::Unhandled();
}

FReply UUDWindowUserWidget::OnMovement(FVector2D recentMousePosition)
{
	FVector2D newWindowPosition = recentMousePosition - PressOffset;
	UWidgetLayoutLibrary::SlotAsCanvasSlot(WindowSizeBox)->SetPosition(newWindowPosition);
	return FReply::Handled();
}

FReply UUDWindowUserWidget::OnResize(FVector2D recentMousePosition)
{
	FVector2D windowPosition = UWidgetLayoutLibrary::SlotAsCanvasSlot(WindowSizeBox)->GetPosition();
	if (UKismetMathLibrary::InRange_FloatFloat(windowPosition.X, MinimumSize.X, MaximumSize.X))
	{
		WindowSizeBox->SetWidthOverride(windowPosition.X);
	}
	if (UKismetMathLibrary::InRange_FloatFloat(windowPosition.Y, MinimumSize.Y, MaximumSize.Y))
	{
		WindowSizeBox->SetHeightOverride(windowPosition.Y);
	}
	return FReply::Handled();
}

void UUDWindowUserWidget::SnapToGrid()
{
	// Position
	FVector2D windowPosition = UWidgetLayoutLibrary::SlotAsCanvasSlot(WindowSizeBox)->GetPosition();
	FVector2D newWindowPosition;
	newWindowPosition.X = UKismetMathLibrary::GridSnap_Float(windowPosition.X, GridSnapSize);
	newWindowPosition.Y = UKismetMathLibrary::GridSnap_Float(windowPosition.Y, GridSnapSize);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(WindowSizeBox)->SetPosition(newWindowPosition);
	UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: Snap %f %f."), newWindowPosition.X, newWindowPosition.Y);
	// Width
	float windowWidth = WindowSizeBox->GetWidthOverride();
	float newWindowWidth = UKismetMathLibrary::GridSnap_Float(windowWidth, GridSnapSize);
	WindowSizeBox->SetWidthOverride(newWindowWidth);
	// Height
	float windowHeight = WindowSizeBox->GetHeightOverride();
	float newWindowHeight = UKismetMathLibrary::GridSnap_Float(windowHeight, GridSnapSize);
	WindowSizeBox->SetHeightOverride(newWindowHeight);
	// Normalization
	NormalizeToScreen(newWindowPosition);
}

#include "Components/GridPanel.h"
void UUDWindowUserWidget::NormalizeToScreen(FVector2D snappedPosition)
{
	FVector2D screenBounded;
	// We need real screen size to properly set it away from bottom right corner.
	FVector2D screenSize = GetCachedGeometry().GetLocalSize();
	UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: Screen %f %f."), screenSize.X, screenSize.Y);
	// We take size that is foced on it.
	FVector2D windowSize = ContentGrid->GetDesiredSize();
	UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: Window %f %f."), windowSize.X, windowSize.Y);
	// Clamp results:
	// top left corner snap at 0.0
	// bottom right snap, if is leaving the screen by being larger then sceenSize - windowSize
	// window snap at any other coordinate
	screenBounded.X = FMath::Clamp(snappedPosition.X, 0.0, screenSize.X - windowSize.X);
	screenBounded.Y = FMath::Clamp(snappedPosition.Y, 0.0, screenSize.Y - windowSize.Y);
	// Apply position
	UWidgetLayoutLibrary::SlotAsCanvasSlot(WindowSizeBox)->SetPosition(screenBounded);
	UE_LOG(LogTemp, Log, TEXT("UUDWindowUserWidget: Normalized %f %f."), screenBounded.X, screenBounded.Y);
}