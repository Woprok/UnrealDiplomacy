// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDMenuUserWidget.generated.h"

// Fordward Declarations

class UTextBlock;
class UButton;
class UUDMenuViewModel;

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API UUDMenuUserWidget : public UUDUserWidget
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
	void SetBlueprintViewModel(UUDMenuViewModel* model);
protected:
	virtual void BindDelegates() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CreateGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CreateGameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> JoinGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> JoinGameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SettingsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SettingsText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuitText;
	UPROPERTY()
	TObjectPtr<UUDMenuViewModel> ViewModel;
private:
	UFUNCTION()
	void CreateGame();
	UFUNCTION()
	void JoinGame();
	UFUNCTION()
	void Settings();
	UFUNCTION()
	void Quit();
};