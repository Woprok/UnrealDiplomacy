// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDLobbyViewModel.generated.h"

/**
 * Lobby Widget
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLobbyViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Back();
	UFUNCTION()
	void Leave();
	UFUNCTION()
	void Start();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LobbyTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameHeaderText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LeaveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StartText;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
private:
	/**
	 * Starts current session.
	 */
	void StartGame();
	/**
	 * Host requested for this session and level to be destroyed.
	 * All players will be disconnected and returned to menu.
	 * Clients will most likely have to handle network error.
	 * End current game.
	 */
	void QuitGame();
	/**
	 * Client requested to leave session and level. 
	 * Leaves current game.
	 */
	void LeaveGame();
	/**
	 * Callback for session start.
	 */
	UFUNCTION()
	void OnSessionStarted(bool success);
private:
	// MVVM Setters & Getters
	void SetLobbyTitleText(FText newLobbyTitleText);
	FText GetLobbyTitleText() const;
	void SetNameHeaderText(FText newNameHeaderText);
	FText GetNameHeaderText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
	void SetLeaveText(FText newLeaveText);
	FText GetLeaveText() const;
	void SetStartText(FText newStartText);
	FText GetStartText() const;
};