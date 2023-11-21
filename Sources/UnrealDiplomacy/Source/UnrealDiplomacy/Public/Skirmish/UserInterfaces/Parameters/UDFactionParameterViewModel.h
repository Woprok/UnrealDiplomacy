// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDFactionParameterViewModel.generated.h"

// Forward Declarations

struct FUDFactionMinimalinfo;
struct FUDFactionParameter;

DECLARE_MULTICAST_DELEGATE(FUDParameterChanged);

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the parameter option. */
	void SetContent(FUDFactionParameter content);
	/** Getter for edited value. */
	int32 GetAsValue();
public:
	// Button Functions
	UFUNCTION()
	void PreviousFaction();
	UFUNCTION()
	void NextFaction();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionTitleText;
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
public:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Tries to retrieve desired selected item, on fail returns first element.
	 * If options list is empty returns invalid item.
	 */
	FUDFactionMinimalInfo GetSelectedOrDefault(int32 desiredSelectedItem);
private:
	void ChangeAttempted();
private:
	// MVVM Setters & Getters
	void SetFactionTitleText(FText newFactionTitleText);
	FText GetFactionTitleText() const;
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
	FUDFactionParameter Content;
	int32 SelectedFaction;
	int32 SelectedFactionIndex;
};