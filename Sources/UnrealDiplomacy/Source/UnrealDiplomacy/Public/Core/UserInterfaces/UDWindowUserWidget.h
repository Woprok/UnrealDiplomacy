// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDUserWidget.h"
#include "UDWindowUserWidget.generated.h"

// Forward Declarations

class USizeBox;
class UGridPanel;
class UCanvasPanel;

/**
 * Common ancestors for all widgets that are acting as moveable window.
 * This implementation requires following named widgets to be part of widget tree:
 * UCanvasPanel named ScreenCanvas, 
 * USizeBox named WindowSizeBox,
 * UGridPanel named ContentGrid.
 * 
 * TODO finish implementation of resize.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDWindowUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
protected:
	/**
	 * Owner of the WindowSizeBox.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ScreenCanvas;
	/**
	 * Required for movement and resize.
	 * Owner of the ContentGrid.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> WindowSizeBox;
	/**
	 * Required for normalizing.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UGridPanel> ContentGrid;
private:
	// Movement
	FVector2D InitialMousePosition;
	FVector2D WindowPosition;
	FVector2D PressOffset;
	bool IsMoving = false;
	FKey MovementButton = EKeys::LeftMouseButton;
	FReply StartMovement(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply OnMovement(FVector2D recentMousePosition);
	FReply EndMovement(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	// Resizing
	bool IsResizing = false;
	FKey ResizeButton = EKeys::RightMouseButton;
	FVector2D MinimumSize = FVector2D(128, 72);
	FVector2D MaximumSize = FVector2D(384, 216);
	FReply StartResize(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply OnResize(FVector2D recentMousePosition);
	FReply EndResize(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	// Snapping and Normalization
	float GridSnapSize = 16;
	void SnapToGrid();
	void NormalizeToScreen(FVector2D snappedPosition);
};