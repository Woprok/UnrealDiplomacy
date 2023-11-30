// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealExecutionTabViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
class UUDActionItemViewModel;

/**
 * Tab for showing and editing actions that are part of deal.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealExecutionTabViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the action list. */
	void InvalidateContent(FUDDealMinimalInfo content);
	/** Unset content of the action list. */
	void SetContent(FUDDealMinimalInfo content);
public:
	// Button Functions
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList ActionItemList;
	// Events
public:
	virtual void Setup() override;
	UFUNCTION()
	virtual void Refresh() override;
private:
	/** Fill with current deal points. */
	void UpdateActionList();
private:
	// MVVM Setters & Getters
	void SetActionItemList(FUDViewModelList newActionItemList);
	FUDViewModelList GetActionItemList() const;
private:
	// Fields
	FUDDealMinimalInfo Content;
	// Collections
	FName ActionItemViewModelCollectionName = TEXT("ActionItemCollection");
	TSubclassOf<UUDViewModel> ActionItemViewModelType;
	TArray<TObjectPtr<UUDActionItemViewModel>> ActionItemViewModelCollection;
};