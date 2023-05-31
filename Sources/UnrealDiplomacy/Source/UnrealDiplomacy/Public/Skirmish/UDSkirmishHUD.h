// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDHUD.h"
#include "UDSkirmishHUD.generated.h"

/**
 * UI for lobby & game.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishHUD : public AUDHUD
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted SkirmishHUD.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<AUDSkirmishHUD> Get(TObjectPtr<UWorld> world);
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ScreenNames")
	FName LobbyScreen = TEXT("LobbyScreen");
};