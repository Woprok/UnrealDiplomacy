// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDUserWidget.h"
#include "UDWindowUserWidget.generated.h"

// Forward Declarations

class UCanvasPanel;

/**
 * Common ancestors for all widgets that are acting as moveable window.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDWindowUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ScreenCanvas;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> InnerCanvas;
private:
	void MousePositionChanged();
	FVector2D GetCurrentPosition();
	bool IsMoving = false;
	FVector2D OriginalPosition;
};