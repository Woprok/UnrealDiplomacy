// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDSecondaryPointItemViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;
class UUDTertiaryPointItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDTertiaryPointItemSourceUpdated, const TArray<TObjectPtr<UUDTertiaryPointItemViewModel>>& itemViewModels);

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSecondaryPointItemViewModel : public UUDViewModel
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
	void CreateSecondaryPoint();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateSecondaryPointText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsValidContentValue;
	// Events
	FUDTertiaryPointItemSourceUpdated PointItemSourceUpdatedEvent;
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
	void SetCreateSecondaryPointText(FText newCreateSecondaryPointText);
	FText GetCreateSecondaryPointText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
private:
	// Fields
	FUDDealPointMinimalInfo Content;
	// Collections
	FName PointItemViewModelCollectionName = TEXT("TertiaryPointItemCollection");
	TSubclassOf<UUDViewModel> PointItemViewModelType;
	TArray<TObjectPtr<UUDTertiaryPointItemViewModel>> PointItemViewModelCollection;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};