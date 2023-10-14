// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDPolicySelectItemViewModel.generated.h"

// Forward Declarations

struct FUDPolicySelectItemInfo;

/**
 * Single selectable item.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDPolicySelectItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Reset content of the interaction option to invalid.
	 * Disabling interations.
	 */
	void InvalidateContent(FUDPolicySelectItemInfo content);
	/**
	 * Set content of the interaction option.
	 */
	void SetContent(FUDPolicySelectItemInfo content);
public:
	// Button Functions
	/**
	 * Interacting with this item.
	 */
	UFUNCTION()
	void Select();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SelectItemText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsItemSelectedValue;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetSelectItemText(FText newSelectItemText);
	FText GetSelectItemText() const;
	void SetIsItemSelectedValue(bool newIsItemSelectedValue);
	bool GetIsItemSelectedValue() const;
private:
	// Fields
	FUDPolicySelectItemInfo Content;
};