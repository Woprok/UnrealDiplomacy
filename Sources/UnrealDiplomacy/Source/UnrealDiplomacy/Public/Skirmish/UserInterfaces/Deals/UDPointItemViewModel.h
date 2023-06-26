// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDPointItemViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDDealPointMinimalInfo content);
public:
	// Button Functions
	UFUNCTION()
	void CreatePoint();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreatePointText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsValidContentValue;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetCreatePointText(FText newCreatePointText);
	FText GetCreatePointText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
private:
	// Fields
	FUDDealPointMinimalInfo Content;
};