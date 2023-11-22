// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTertiaryPointItemViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;
class UUDPointContentViewModel;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTertiaryPointItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the deal point content. */
	void SetContent(FUDDealPointMinimalInfo content, bool isValid);
public:
	// Button Functions
	UFUNCTION()
	void CreateTertiaryPoint();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateTertiaryPointText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsValidContentValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent PointContent;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/** Fill current point with data. */
	void UpdatePointContent();
private:
	// MVVM Setters & Getters
	void SetCreateTertiaryPointText(FText newCreateTertiaryPointText);
	FText GetCreateTertiaryPointText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
	void SetPointContent(FUDViewModelContent newPointContent);
	FUDViewModelContent GetPointContent() const;
private:
	// Fields
	FUDDealPointMinimalInfo Content;
	// Instances
	FName PointContentViewModelInstanceName = TEXT("TertiaryPointContentInstance");
	TSubclassOf<UUDViewModel> PointContentViewModelType;
	TObjectPtr<UUDPointContentViewModel> PointContentViewModelInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};