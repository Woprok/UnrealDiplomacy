// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDGameOverViewModel.generated.h"

// Forward Declarations

/**
 * Contains everything necessary to display the winner, loser, gameover presentations.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameOverViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Return();
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
	void SetGameOverTitleText(FText newGameOverTitleText);
	FText GetGameOverTitleText() const;
	void SetWinnerFactionText(FText newWinnerFactionText);
	FText GetWinnerFactionText() const;
	void SetReturnToMenuText(FText newReturnToMenuText);
	FText GetReturnToMenuText() const;
	void SetIsHostValue(bool newIsHostValue);
	bool GetIsHostValue() const;
	void SetIsWinnerValue(bool newIsWinnerValue);
	bool GetIsWinnerValue() const;
	void SetIsPlayerValue(bool newIsPlayerValue);
	bool GetIsPlayerValue() const;
};