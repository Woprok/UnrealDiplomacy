// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDSecondaryPointItemViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;
class UUDTertiaryPointItemViewModel;
class UUDPointContentViewModel;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSecondaryPointItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the deal point content. */
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
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList PointItemList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent PointContent;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/** Fill with current deal points. */
	void UpdatePointList();
	/** Fill current point with data. */
	void UpdatePointContent();
private:
	// MVVM Setters & Getters
	void SetCreateSecondaryPointText(FText newCreateSecondaryPointText);
	FText GetCreateSecondaryPointText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
	void SetPointItemList(FUDViewModelList newPointItemList);
	FUDViewModelList GetPointItemList() const;
	void SetPointContent(FUDViewModelContent newPointContent);
	FUDViewModelContent GetPointContent() const;
private:
	// Fields
	FUDDealPointMinimalInfo Content;
	// Instances
	FName PointContentViewModelInstanceName = TEXT("SecondaryPointContentInstance");
	TSubclassOf<UUDViewModel> PointContentViewModelType;
	TObjectPtr<UUDPointContentViewModel> PointContentViewModelInstance;
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