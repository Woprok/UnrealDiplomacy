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
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void StartGame()
	{
		ActionModel->RequestAction(ActionModel->GetEndTurnAction());
	}
protected:
};