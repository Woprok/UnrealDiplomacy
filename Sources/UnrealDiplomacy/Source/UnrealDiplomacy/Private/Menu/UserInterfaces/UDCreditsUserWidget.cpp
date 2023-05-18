// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreditsUserWidget.h"
#include "Menu/UserInterfaces/UDCreditsViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Menu/UDMenuHUD.h"

void UUDCreditsUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDCreditsViewModel* creditsViewModel = CastChecked<UUDCreditsViewModel>(viewModel.Get());
	ViewModel = creditsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDCreditsUserWidget::BindWidgets()
{
	CreditsTitleTextWidget = GetWidget<UTextBlock>(TEXT("CreditsTitleText"));
	ContentTextWidget = GetWidget<UTextBlock>(TEXT("ContentText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
}

void UUDCreditsUserWidget::BindDelegates()
{
	BackButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDCreditsUserWidget::Back);
}

void UUDCreditsUserWidget::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDCreditsUserWidget: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->SettingsScreen);
}