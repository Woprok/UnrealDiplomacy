// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDViewModelManager.generated.h"

// Forward Declarations

class UUDViewModel;

/**
 * Manager for maintaining instances of view models.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDViewModelManager : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Calls update on all global view models.
	 */
	void ForceUpdate();
	/**
	 * Retrieve global view model based on name.
	 */
	TObjectPtr<UUDViewModel> Get(const FName& name);
	/**
	 * Add new view model to list of all updated models.
	 */
	void Register(FName name, TSubclassOf<UUDViewModel> viewModelType);
protected:
	/**
	 * Initializes new instance of specified view model class.
	 */
	TObjectPtr<UUDViewModel> Create(TSubclassOf<UUDViewModel> viewModelType);
	/**
	 * Current collection of available global view models.
	 * Each is instanced exactly once.
	 */
	UPROPERTY()
	TMap<FName, TObjectPtr<UUDViewModel>> ViewModels;
};