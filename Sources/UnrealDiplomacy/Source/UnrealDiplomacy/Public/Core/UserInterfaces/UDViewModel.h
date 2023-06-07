// Copyright Miroslav Valach
// TODO Rework full update and potentially remove it.

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
	 * Run initialize without update.
	 */
	void FullInitialize();
	/**
	 * Notifies subscribers about incoming update.
	 * Executes update and notifies again about the end of the update.
	 * This updates is used for complete setup and full change.
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
	 * Invoked for first update and for reloads.
	 * Used for creating defaults such as these that are based on application settings.
	 */
	virtual void Initialize();
	/**
	 * Invoked for each update.
	 * Should react to any possible data change.
	 */
	virtual void Update();
	/**
	 * Invoked after each set model call.
	 */
	virtual void OnModelChanged();
protected:
	/**
	 * Model is used to communicate with the game logic.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> Model = nullptr;
	// HACK: Base class uses == and due to that FText is not supported.
	// TODO: Change this after base class change or use custom macro.
	using UMVVMViewModelBase::SetPropertyValue;
	/** 
	 * Set the new value and notify if the property value changed. 
	 */
	bool SetPropertyValue(FText& Value, const FText& NewValue, UE::FieldNotification::FFieldId FieldId)
	{
		if (Value.EqualTo(NewValue))
		{
			return false;
		}

		Value = NewValue;
		BroadcastFieldValueChanged(FieldId);
		return true;
	}
};