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
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static FText Conv_BiomeTypeToText(int32 InInteger)
	{
		if (InInteger == 69)
		{
			return FText::FromString("GRASSLAND");
		}
		if (InInteger == 42)
		{
			return FText::FromString("MOUNTAIN");
		}
		return FText::FromString("DEBUG");
	}
	// TODO PRECOMPILE FORMAT TEXT
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static FText Conv_AppendIntegerToText(FString InText, int32 InInteger)
	{
		return FText::FromString(FString::Format(TEXT("{0} ({1})"), { InText, InInteger }));
	}
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static FText Conv_IntegerPairCurrentMaxToText(int32 InCurrent, int32 InMax)
	{
		return FText::FromString(FString::Format(TEXT("{0} / {1}"), { InCurrent, InMax }));
	}
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static ESlateVisibility Conv_BoolToVisibility(bool InIsVisible)
	{
		if (InIsVisible)
		{
			return ESlateVisibility::Visible;
		}
		return ESlateVisibility::Collapsed;
	}
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static ESlateVisibility Conv_BoolToInVisibility(bool InIsVisible)
	{
		if (InIsVisible)
		{
			return ESlateVisibility::Collapsed;
		}
		return ESlateVisibility::Visible;
	}
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static FText Conv_DealStateToText(EUDDealSimulationState InState)
	{
		switch (InState)
		{
		case EUDDealSimulationState::Undefined:
			return FText::FromString("Undefined");
			break;
		case EUDDealSimulationState::CreatingDraft:
			return FText::FromString("Creating Draft");
			break;
		case EUDDealSimulationState::Assembling:
			return FText::FromString("Assembling");
			break;
		case EUDDealSimulationState::ExtendingDraft:
			return FText::FromString("Extending Draft");
			break;
		case EUDDealSimulationState::DemandsAndRequests:
			return FText::FromString("Demands And Requests");
			break;
		case EUDDealSimulationState::Bidding:
			return FText::FromString("Bidding");
			break;
		case EUDDealSimulationState::FinalizingDraft:
			return FText::FromString("Finalizing Draft");
			break;
		case EUDDealSimulationState::Vote:
			return FText::FromString("Vote");
			break;
		case EUDDealSimulationState::Resolution:
			return FText::FromString("Resolution");
			break;
		case EUDDealSimulationState::Result:
			return FText::FromString("Result");
			break;
		default:
			return FText::FromString("default");
			break;
		}
	}
	UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	static FText Conv_DealResultToText(EUDDealSimulationResult InResult)
	{
		switch (InResult)
		{
		case EUDDealSimulationResult::Undefined:
			return FText::FromString("Undefined");
			break;
		case EUDDealSimulationResult::Opened:
			return FText::FromString("Opened");
			break;
		case EUDDealSimulationResult::Passed:
			return FText::FromString("Passed");
			break;
		case EUDDealSimulationResult::Vetoed:
			return FText::FromString("Vetoed");
			break;
		case EUDDealSimulationResult::Disassembled:
			return FText::FromString("Disassembled");
			break;
		case EUDDealSimulationResult::Closed:
			return FText::FromString("Closed");
			break;
		default:
			return FText::FromString("default");
			break;
		}
	}
	//UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	//static FSlateColor Conv_BoolToColour(bool InIs)
	//{
	//	if (InIs)
	//	{
	//		return FSlateColor;
	//	}
	//	return FSlateColor;
	//}
	//UFUNCTION(BlueprintPure, meta = (BlueprintAutocast), Category = "Convertors|Views")
	//static FSlateColor Conv_BoolToInverseColour(bool InIsNot)
	//{
	//	if (InIsNot)
	//	{
	//		return FSlateColor;
	//	}
	//	return FSlateColor;
	//}
};

/**
 * TODO check if someone found or implemented function binding to events
 */
UCLASS(Blueprintable, BlueprintType)
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

/**
 * TODO REMOVE THIS as its useless and we are stuck with needing access to ActionModel for any interaction
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDStaticViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetAuthority(UUDActionAdministrator* model)
	{
		ActionModel = model;
	}
protected:
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> ActionModel = nullptr;
};