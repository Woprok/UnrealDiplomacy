// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/UserInterfaces/UDViewModelManager.h"

TObjectPtr<AUDSkirmishHUD> AUDSkirmishHUD::Get(TObjectPtr<UWorld> world)
{
	TObjectPtr<APlayerController> pc = UGameplayStatics::GetPlayerController(world, 0);
	check(pc != nullptr);
	TObjectPtr<AHUD> hud = pc->GetHUD();
	check(hud != nullptr);
	return CastChecked<AUDSkirmishHUD>(hud);
}

void AUDSkirmishHUD::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
	SwitchScreen(LobbyScreen);
	ForceInitialize();
}

void AUDSkirmishHUD::RequestNotifyOnTileSelected(FIntPoint tile)
{
	OnTileSelectedEvent.Broadcast(tile);
	BP_OnTileSelected(tile);
}

void AUDSkirmishHUD::RequestNotifyOnFactionSelected(int32 factionId)
{
	OnFactionSelectedEvent.Broadcast(factionId);
	BP_OnFactionSelected(factionId);
}

void AUDSkirmishHUD::OnComponentsInitialized()
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: On Components Initialize."));
	TObjectPtr<AUDSkirmishPlayerController> pc = AUDSkirmishPlayerController::Get(GetWorld());
	ViewModelManager->SetModelManager(pc->GetAdministrator());
}