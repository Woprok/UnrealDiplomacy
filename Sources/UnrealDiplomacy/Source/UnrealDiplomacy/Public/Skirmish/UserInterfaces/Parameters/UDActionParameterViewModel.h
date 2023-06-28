// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDActionParameterViewModel.generated.h"

// Forward Declarations

struct FUDActionMinimalInfo;
struct FUDActionParameter;

DECLARE_MULTICAST_DELEGATE(FUDParameterChanged);

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDActionParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the parameter option. */
	void SetContent(FUDActionParameter content);
	/** Getter for edited value. */
	int32 GetAsValue();
public:
	// Button Functions
	UFUNCTION()
	void PreviousAction();
	UFUNCTION()
	void NextAction();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ActionTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PreviousText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NextText;
	// Events
	FUDParameterChanged OnChangeEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Tries to retrieve desired selected item, on fail returns first element.
	 * If options list is empty returns invalid item.
	 */
	FUDActionMinimalInfo GetSelectedOrDefault(int32 desiredSelectedItem);
private:
	void ChangeAttempted();
private:
	// MVVM Setters & Getters
	void SetActionTitleText(FText newActionTitleText);
	FText GetActionTitleText() const;
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetToolTipText(FText newToolTipText);
	FText GetToolTipText() const;
	void SetPreviousText(FText newPreviousText);
	FText GetPreviousText() const;
	void SetNextText(FText newNextText);
	FText GetNextText() const;
private:
	// Fields
	FUDActionParameter Content;
	int32 SelectedAction;
	int32 SelectedActionIndex;
};