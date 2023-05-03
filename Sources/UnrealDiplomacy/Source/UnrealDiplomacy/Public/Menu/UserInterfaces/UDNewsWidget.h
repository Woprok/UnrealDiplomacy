// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDNewsWidget.generated.h"

// Forward Declaratiosn

class UTextBlock;
class UButton;
class UUDNewsViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDNewsWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables to define view model used by this widet for delegates.
	 */
	virtual void SetViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDNewsViewModel* model);
protected:
	virtual void BindDelegates() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
	UPROPERTY()
	TObjectPtr<UUDNewsViewModel> ViewModel;
private:
	UFUNCTION()
	void Close();
};