// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "UDDealProcessViewModel.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealProcessViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:

public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// This is visible during the game and shows current info
			//UpdateInteractions(ActionModel->GetPendingRequests());
		}
		else
		{
			// This should not be visible ?
		}
	}
protected:

private:

};