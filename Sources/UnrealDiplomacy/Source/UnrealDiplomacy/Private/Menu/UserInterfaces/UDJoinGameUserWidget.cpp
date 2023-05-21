// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameUserWidget.h"
#include "Menu/UserInterfaces/UDJoinGameViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Menu/UDMenuHUD.h"

void UUDJoinGameUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDJoinGameViewModel* joinGameViewModel = CastChecked<UUDJoinGameViewModel>(viewModel.Get());
	ViewModel = joinGameViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDJoinGameUserWidget::BindWidgets()
{
	JoinGameTitleTextWidget = GetWidget<UTextBlock>(TEXT("JoinGameTitleText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	RefreshTextWidget = GetWidget<UTextBlock>(TEXT("RefreshText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
}

void UUDJoinGameUserWidget::BindDelegates()
{
	BackButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDJoinGameUserWidget::Back);
}

void UUDJoinGameUserWidget::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameUserWidget: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}