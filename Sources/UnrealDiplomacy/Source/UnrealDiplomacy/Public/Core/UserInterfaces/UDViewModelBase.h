// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"

#include "Components/SlateWrapperTypes.h"

#include "UDViewModelBase.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "ConvertorsLibrary"))
class UNREALDIPLOMACY_API UUDViewConvertors : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is True > 0 (Integer)", CompactNodeTitle = ">0", BlueprintAutocast), Category = "Convertors|Views")
	static bool Conv_CheckLargerThan0ToBool(int32 InInteger)
	{
		return InInteger > 0;
	}
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Visible > 0 (Integer)", CompactNodeTitle = ">0", BlueprintAutocast), Category = "Convertors|Views")
	static ESlateVisibility Conv_CheckLargerThan0ToVisibility(int32 InInteger)
	{
		if (InInteger > 0)
		{
			return ESlateVisibility::Visible;
		}
		return ESlateVisibility::Collapsed;
	}
};

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