// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsUserWidget.h"
#include "Components/Button.h"
#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "Kismet/GameplayStatics.h"
//#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"

void UUDSettingsUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDSettingsViewModel* settingsViewModel = CastChecked<UUDSettingsViewModel>(viewModel.Get());
	ViewModel = settingsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDSettingsUserWidget::BindDelegates()
{
	BackButton->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Back);
	SaveButton->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Save);
}

void UUDSettingsUserWidget::Back()
{
	auto hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
	CastChecked<AUDMenuHUD>(hud)->SwitchScreen("Menu");
}

void UUDSettingsUserWidget::Save()
{
	auto hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
	CastChecked<AUDMenuHUD>(hud)->SwitchScreen("Menu");
}