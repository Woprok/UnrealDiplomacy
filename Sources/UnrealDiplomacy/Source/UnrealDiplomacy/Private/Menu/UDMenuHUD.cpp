// Copyright Miroslav Valach

#include "Menu/UDMenuHUD.h"
#include "Kismet/GameplayStatics.h"

TObjectPtr<AUDMenuHUD> AUDMenuHUD::Get(TObjectPtr<UWorld> world)
{
	TObjectPtr<APlayerController> pc = UGameplayStatics::GetPlayerController(world, 0);
	check(pc != nullptr);
	TObjectPtr<AHUD> hud = pc->GetHUD();
	check(hud != nullptr);
	return CastChecked<AUDMenuHUD>(hud);
}