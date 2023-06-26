// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealExecutionTabViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
class UUDActionItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDActionItemSourceUpdated, const TArray<TObjectPtr<UUDActionItemViewModel>>& itemViewModels);

/**
 * Tab for showing and editing actions that are part of deal.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealExecutionTabViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter editor.
	 */
	void InvalidateContent(FUDDealMinimalInfo content);
	/**
	 * Set content of the parameter editor.
	 */
	void SetContent(FUDDealMinimalInfo content);
public:
	// Button Functions
	// MVVM Fields
	// Events
	FUDActionItemSourceUpdated ActionItemSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Fill with current deal points.
	 */
	void UpdateActionList();
private:
	// MVVM Setters & Getters
private:
	// Fields
	FUDDealMinimalInfo Content;
	// Collections
	FName ActionItemViewModelCollectionName = TEXT("ActionItemCollection");
	TSubclassOf<UUDViewModel> ActionItemViewModelType;
	TArray<TObjectPtr<UUDActionItemViewModel>> ActionItemViewModelCollection;
};