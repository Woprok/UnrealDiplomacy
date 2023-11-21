// Copyright Miroslav Valach
// TODO Rework full update and potentially remove it.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UDViewModel.generated.h"

// Forward Declarations

class UUDActionAdministrator;
class UUDViewModel;

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
	 * Prepares UI for being displayed.
	 * Initializes dependencies (e.g. for content widgets, empty list..., default strings ).
	 * Thus it's using generic application settings over any game specific data (avoid accessing these).
	 * Primarily defines everything to prevent any undefined behaviour on all other viewmodel calls.
	 */
	virtual void Setup();
	/**
	 * Starts refresh on the viewmodel, potentially updating current content.
	 * This is supposed to be called after Setup and after view is initialized.
	 * Thus this might be called before specific content was passed to it and should always check it has content.
	 * Menu ViewModels might self SetContent in this.
	 * Most Game ViewModels receive SetContent from events or other ViewModels.
	 */
	virtual void Refresh();
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