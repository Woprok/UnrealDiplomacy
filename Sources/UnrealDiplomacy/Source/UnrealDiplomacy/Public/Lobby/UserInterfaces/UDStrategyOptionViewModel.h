// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDStrategyOptionViewModel.generated.h"

// Forward Declarations

struct FUDStratagemPickableInfo;

/**
 * ViewModel for single strategy option used in tile view.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDStrategyOptionViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the strategy option. */
	void SetContent(FUDStratagemPickableInfo content);
public:	
	// Button Functions
	/** Attempts to join the new session, hides UI until the end of join operation. */
	UFUNCTION()
	void Selected();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText OptionToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsSelectedValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetOptionToolTipText(FText newOptionToolTipText);
	FText GetOptionToolTipText() const;
	void SetIsSelectedValue(bool newIsSelectedValue);
	bool GetIsSelectedValue() const;
private:
	// Fields
	FUDStratagemPickableInfo Content;
};