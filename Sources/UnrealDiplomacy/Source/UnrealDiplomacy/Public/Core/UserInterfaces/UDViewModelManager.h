// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDViewModelManager.generated.h"

// Forward Declarations

class UUDViewModel;

/**
 * Common ancestors for all view models.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDViewModelManager : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Add new view model to list of all updated models.
	 */
	void RegisterViewModel(TObjectPtr<UUDViewModel> viewModel);

	void UpdateViewModels();

	TObjectPtr<UUDViewModel> Get(FName name);
protected:
	UPROPERTY()
	TArray<TObjectPtr<UUDViewModel>> ViewModels;
};