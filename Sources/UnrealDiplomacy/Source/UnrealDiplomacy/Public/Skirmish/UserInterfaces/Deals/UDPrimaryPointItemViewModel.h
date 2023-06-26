// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDPrimaryPointItemViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;
class UUDSecondaryPointItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDSecondaryPointItemSourceUpdated, const TArray<TObjectPtr<UUDSecondaryPointItemViewModel>>& itemViewModels);

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPrimaryPointItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDDealPointMinimalInfo content, bool isValid);
public:
	// Button Functions
	UFUNCTION()
	void CreatePoint();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreatePointText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsValidContentValue;
	// Events
	FUDSecondaryPointItemSourceUpdated PointItemSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Fill with current deal points.
	 */
	void UpdatePointList();
private:
	// MVVM Setters & Getters
	void SetCreatePointText(FText newCreatePointText);
	FText GetCreatePointText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
private:
	// Fields
	FUDDealPointMinimalInfo Content;
	// Collections
	FName PointItemViewModelCollectionName = TEXT("SecondaryPointItemCollection");
	TSubclassOf<UUDViewModel> PointItemViewModelType;
	TArray<TObjectPtr<UUDSecondaryPointItemViewModel>> PointItemViewModelCollection;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};