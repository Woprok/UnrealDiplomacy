// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDTurnViewModel.generated.h"

// Forward Declarations

/**
 * Contains all data related presenting informations and actions related to turns.
 */
class UNREALDIPLOMACY_API UUDTurnViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void FinishTurn();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText GameOverTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText WinnerFactionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ReturnToMenuText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsHostValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsPlayerValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsWinnerValue;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Returns to menu by calling quit session.
	 */
	void ReturnToMenu();
	/**
	 * Determines correct visual presentation for game over screen.
	 */
	void ChangeGameOverPresentation();
private:
	// MVVM Setters & Getters
};