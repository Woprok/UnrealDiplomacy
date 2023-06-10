// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

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
	CurrentScreen = LobbyScreen;
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
	Super::OnComponentsInitialized();
	if (IsComponentInitialized)
		return;
	IsComponentInitialized = true;
	TObjectPtr<AUDSkirmishPlayerController> pc = AUDSkirmishPlayerController::Get(GetWorld());
	TObjectPtr<UUDActionAdministrator> model = pc->GetAdministrator();
	model->OnDataReloadedEvent.AddUniqueDynamic(this, &AUDSkirmishHUD::OnGameDataChanged);
	model->OnDataChangedEvent.AddUniqueDynamic(this, &AUDSkirmishHUD::OnGameDataChanged);
	ViewModelManager->SetModelManager(model);
}

void AUDSkirmishHUD::OnGameDataChanged()
{
	TObjectPtr<AUDSkirmishPlayerController> pc = AUDSkirmishPlayerController::Get(GetWorld());
	TObjectPtr<UUDActionAdministrator> model = pc->GetAdministrator();

	FName nextScreen;
	switch (model->GetMatchStateInfo())
	{
	case EUDGameStateInfo::Lobby:
		nextScreen = LobbyScreen;
		break;
	case EUDGameStateInfo::Match:
		nextScreen = GameScreen;
		break;
	case EUDGameStateInfo::Scoreboard:
		nextScreen = ScoreboardScreen;
		break;
	}
	// Swap only if screens can be changed.
	if (nextScreen == CurrentScreen)
		return;
	// Finally do the swap.
	CurrentScreen = nextScreen;
	SwitchScreen(nextScreen);
}