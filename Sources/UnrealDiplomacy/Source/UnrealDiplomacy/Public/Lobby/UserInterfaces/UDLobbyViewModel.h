// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDLobbyViewModel.generated.h"

/**
 * Lobby Widget
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDLobbyViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanStartGame = false;
public:
	virtual void OnUpdate() override 
	{
		CanStartGame = true;
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void StartGame()
	{
		ActionModel->RequestAction(ActionModel->GetStartGameAction());
	}
protected:
private:
	/**
	 * MVVM Binding.
	 */
	void SetCanStartGame(bool newCanStartGame)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CanStartGame, newCanStartGame);
	}
	/**
	 * MVVM Binding.
	 */
	bool GetCanStartGame() const
	{
		return CanStartGame;
	}
};