// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDLoadingUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UUDLoadingViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDLoadingUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDLoadingViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> LoadingTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> GameplayHintTextWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDLoadingViewModel> ViewModel;
};