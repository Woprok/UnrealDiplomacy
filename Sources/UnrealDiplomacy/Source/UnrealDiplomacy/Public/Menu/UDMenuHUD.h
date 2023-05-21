// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDHUD.h"
#include "UDMenuHUD.generated.h"

/**
 * UI for menu.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDMenuHUD : public AUDHUD
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted MenuHUD.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<AUDMenuHUD> Get(TObjectPtr<UWorld> world);
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName MenuScreen = TEXT("MenuScreen");
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName CreditsScreen = TEXT("CreditsScreen");
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName SettingsScreen = TEXT("SettingsScreen");
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName JoinGameScreen = TEXT("JoinGameScreen");
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName CreateGameScreen = TEXT("CreateGameScreen");
};