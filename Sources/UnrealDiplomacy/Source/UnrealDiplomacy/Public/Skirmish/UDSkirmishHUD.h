// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDHUD.h"
#include "UDSkirmishHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnTileSelected, FIntPoint, tile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDOnFactionSelected, int32, factionId);

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
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;
	/**
	 * Used by menu views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ScreenNames")
	FName LobbyScreen = TEXT("LobbyScreen");
public:
	/**
	 * Invoked by tiles, whenever user clicks on them.
	 */
	FUDOnTileSelected OnTileSelectedEvent;
	/**
	 * Invoked by UI elements that want other UI elements to acknowledge that
	 * faction was selected.
	 */
	FUDOnFactionSelected OnFactionSelectedEvent;
	/**
	 * Notification invokeable by corresponding elements.
	 */
	void RequestNotifyOnTileSelected(FIntPoint tile);
	/**
	 * Notification invokeable by corresponding elements.
	 */
	void RequestNotifyOnFactionSelected(int32 factionId);
	/**
	 * Notification invokeable for BP to handle.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SelectionEvent")
	void BP_OnTileSelected(FIntPoint tile);
	/**
	 * Notification invokeable for BP to handle.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SelectionEvent")
	void BP_OnFactionSelected(int32 factionId);
protected:
	/**
	 * Defines custom model to view model manager.
	 */
	virtual void OnComponentsInitialized() override;
};