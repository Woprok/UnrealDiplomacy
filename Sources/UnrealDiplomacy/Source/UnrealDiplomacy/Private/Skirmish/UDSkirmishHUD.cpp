// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishHUD.h"
#include "Kismet/GameplayStatics.h"

TObjectPtr<AUDSkirmishHUD> AUDSkirmishHUD::Get(TObjectPtr<UWorld> world)
{
	TObjectPtr<APlayerController> pc = UGameplayStatics::GetPlayerController(world, 0);
	check(pc != nullptr);
	TObjectPtr<AHUD> hud = pc->GetHUD();
	check(hud != nullptr);
	return CastChecked<AUDSkirmishHUD>(hud);
}