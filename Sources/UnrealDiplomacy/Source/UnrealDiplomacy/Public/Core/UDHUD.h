// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UDHUD.generated.h"

// Forward Declarations
class UUDUserWidget;
class UUDViewModelManager;

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDHUD : public AHUD
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void ShowWidgets();
	UFUNCTION(BlueprintCallable)
	void CreateWidgets();

	UFUNCTION(BlueprintCallable)
	void CreateViewModels();
	UFUNCTION(BlueprintCallable)
	void BindViewModel();
	UFUNCTION(BlueprintCallable)
	void UpdateViewModel();
	UFUNCTION(BlueprintCallable)
	UUDViewModel* GetViewModel(FName name);

	/**
	 * Prepares new instance of view.
	 */
	void CreateView(TSubclassOf<UUDUserWidget> widgetClass);

	void ShowView(TObjectPtr<UUDUserWidget> widget);
protected:
	/**
	 * All views associated with this HUD.
	 */
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UUDUserWidget>> Views;

	UPROPERTY()
	TArray<TObjectPtr<UUDUserWidget>> Widgets;

	UPROPERTY()
	TObjectPtr<UUDViewModelManager> ViewModels;
};