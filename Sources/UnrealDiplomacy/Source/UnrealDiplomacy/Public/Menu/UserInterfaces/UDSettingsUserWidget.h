// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDSettingsUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDSettingsViewModel;

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSettingsUserWidget : public UUDUserWidget
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
	void SetBlueprintViewModel(UUDSettingsViewModel* model);
protected:
	virtual void BindDelegates() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BackText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SaveButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SaveText;
	UPROPERTY()
	TObjectPtr<UUDSettingsViewModel> ViewModel;
private:
	UFUNCTION()
	void Back();
	UFUNCTION()
	void Save();
};