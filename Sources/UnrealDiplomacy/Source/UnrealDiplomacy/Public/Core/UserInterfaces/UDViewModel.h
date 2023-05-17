// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UDViewModel.generated.h"

// Forward Declarations

class UUDActionAdministrator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDViewModelUpdate);

/**
 * Base ancestor for all shared behaviour.
 * Refer to https://docs.unrealengine.com/5.1/en-US/umg-viewmodel/ for full description.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * Model is required for all view models that are using world state.
	 */
	void SetModel(TObjectPtr<UUDActionAdministrator> model);
	/**
	 * Notifies subscribers about incoming update.
	 * Executes update and notifies again about the end of the update.
	 */
	void FullUpdate();
public:
	/**
	 * Subscribeable event that is invoked before the update.
	 * Allows view to be aware of impending update.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDViewModelUpdate OnUpdateStarting;
	/**
	 * Subscribeable event that is invoked after the update.
	 * Useful for views to update their data to fit current view model.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDViewModelUpdate OnUpdateFinishing;
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update();
protected:
	/**
	 * Model is used to communicate with the game logic.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> Model = nullptr;
};