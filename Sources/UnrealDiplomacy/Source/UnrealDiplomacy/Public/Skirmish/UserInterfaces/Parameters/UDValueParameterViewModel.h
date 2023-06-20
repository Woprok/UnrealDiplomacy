// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDValueParameterViewModel.generated.h"

// Forward Declarations

enum ETextCommit::Type : int;
struct FUDValueParameter;

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDValueParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the parameter option. */
	void SetContent(FUDValueParameter content);
	/** Getter for edited value. */
	int32 GetAsValue();
public:
	// Changes Commited
	UFUNCTION()
	void StartValueEditation(float InValue);
	UFUNCTION()
	void StopValueEditation(float InValue, ETextCommit::Type CommitMethod);
	// Button Functions
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ValueTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MinValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MaxValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float SelectedValue;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetValueTitleText(FText newValueTitleText);
	FText GetValueTitleText() const;
	void SetToolTipText(FText newToolTipText);
	FText GetToolTipText() const;
	void SetMinValue(float newMinValue);
	float GetMinValue() const;
	void SetMaxValue(float newMaxValue);
	float GetMaxValue() const;
	void SetSelectedValue(float newSelectedValue);
	float GetSelectedValue() const;
private:
	// Fields
	FUDValueParameter Content;
	bool IsValueEditing = false;
};