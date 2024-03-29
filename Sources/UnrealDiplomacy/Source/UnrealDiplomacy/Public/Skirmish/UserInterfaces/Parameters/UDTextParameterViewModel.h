// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTextParameterViewModel.generated.h"

// Forward Declarations

enum ETextCommit::Type : int;
struct FUDTextParameter;

DECLARE_MULTICAST_DELEGATE(FUDParameterChanged);

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTextParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the parameter option. */
	void SetContent(FUDTextParameter content);
	/** Getter for edited value. */
	FString GetAsText();
public:
	// Button Functions
	UFUNCTION()
	void StartTextEditation(const FText& InText);
	UFUNCTION()
	void StopTextEditation(const FText& InText, ETextCommit::Type CommitMethod);
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText TextTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SelectedText;
	// Events
	FUDParameterChanged OnChangeEvent;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	void ChangeAttempted();
private:
	// MVVM Setters & Getters
	void SetTextTitleText(FText newTextTitleText);
	FText GetTextTitleText() const;
	void SetToolTipText(FText newToolTipText);
	FText GetToolTipText() const;
	void SetSelectedText(FText newSelectedText);
	FText GetSelectedText() const;
private:
	// Fields
	FUDTextParameter Content;
	bool IsTextEditing = false;
};