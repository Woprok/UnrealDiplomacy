// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDViewModelBase.generated.h"

/**
 * TODO check if someone found or implemented function binding to events
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * Sets reference to Model (this tries follow principle MVVM[ActionAdministrator<->ViewModel<->View])
	 */
	UFUNCTION(BlueprintCallable)
	void SetAuthority(UUDActionAdministrator* model)
	{
		ActionModel = model;
	}
	/**
	 * Public handle for notifying view model to update it's binded values from model.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void OnUpdate() 
	{
		// Executes update on any binded UPROPERTY with NotifyField tag.
	}
protected:
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> ActionModel = nullptr;
};