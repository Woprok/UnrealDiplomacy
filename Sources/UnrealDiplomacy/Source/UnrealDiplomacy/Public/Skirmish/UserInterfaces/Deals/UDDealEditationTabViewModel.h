// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealEditationTabViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
class UUDPointItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDPointItemSourceUpdated, const TArray<TObjectPtr<UUDPointItemViewModel>>& itemViewModels);
/**
 * Window for showing and editing deals.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealEditationTabViewModel : public UUDViewModel
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
	FUDPointItemSourceUpdated PointItemSourceUpdatedEvent;
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
	void UpdatePointList();
private:
	// MVVM Setters & Getters
private:
	// Fields
	FUDDealMinimalInfo Content;
	// Collections
	FName PointItemViewModelCollectionName = TEXT("PointItemCollection");
	TSubclassOf<UUDViewModel> PointItemViewModelType;
	TArray<TObjectPtr<UUDPointItemViewModel>> PointItemViewModelCollection;
};