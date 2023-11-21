// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDTurnViewModel.generated.h"

// Forward Declarations

/**
 * Contains all data related presenting informations and actions related to turns.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTurnViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void FinishTurn();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RegentTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RegentText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RegentToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RemainingText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FinishTurnText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FinishTurnToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText TurnText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsRegent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsFinished;
public:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates all parts of turn presentation to latest change.
	 */
	void UpdateTurnPresentation();
private:
	// MVVM Setters & Getters
	void SetRegentTitleText(FText newRegentTitleText);
	FText GetRegentTitleText() const;
	void SetRegentText(FText newRegentText);
	FText GetRegentText() const;
	void SetRegentToolTipText(FText newRegentToolTipText);
	FText GetRegentToolTipText() const;
	void SetRemainingText(FText newRemainingText);
	FText GetRemainingText() const;
	void SetFinishTurnText(FText newFinishTurnText);
	FText GetFinishTurnText() const;
	void SetFinishTurnToolTipText(FText newFinishTurnToolTipText);
	FText GetFinishTurnToolTipText() const;
	void SetTurnText(FText newTurnText);
	FText GetTurnText() const;
	void SetIsRegent(bool newIsRegent);
	bool GetIsRegent() const;
	void SetIsFinished(bool newIsFinished);
	bool GetIsFinished() const;
};