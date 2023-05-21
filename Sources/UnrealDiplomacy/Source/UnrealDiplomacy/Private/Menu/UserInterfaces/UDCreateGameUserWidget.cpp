// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreateGameUserWidget.h"
#include "Menu/UserInterfaces/UDCreateGameViewModel.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Menu/UDMenuHUD.h"

void UUDCreateGameUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDCreateGameViewModel* createGameViewModel = CastChecked<UUDCreateGameViewModel>(viewModel.Get());
	ViewModel = createGameViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDCreateGameUserWidget::BindWidgets()
{
	CreateGameTitleTextWidget = GetWidget<UTextBlock>(TEXT("CreateGameTitleText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	NewGameTextWidget = GetWidget<UTextBlock>(TEXT("NewGameText"));
	SessionNameTextWidget = GetWidget<UTextBlock>(TEXT("SessionNameText"));
	IsLANTextWidget = GetWidget<UTextBlock>(TEXT("IsLANText"));
	IsLANCheckBoxWidget = GetWidget<UCheckBox>(TEXT("IsLANCheckBox"));
	SessionNameEditableTextWidget = GetWidget<UEditableTextBox>(TEXT("SessionNameEditableText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	NewGameButtonWidget = GetWidget<UButton>(TEXT("NewGameButton"));
}

void UUDCreateGameUserWidget::BindDelegates()
{
	BackButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDCreateGameUserWidget::Back);
	NewGameButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDCreateGameUserWidget::NewGame);
	SessionNameEditableTextWidget->OnTextCommitted.AddUniqueDynamic(this, &UUDCreateGameUserWidget::SessionNameChanged);
}

void UUDCreateGameUserWidget::SessionNameChanged(const FText& text, ETextCommit::Type commitMethod)
{
	UE_LOG(LogTemp, Log, TEXT("UUDCreateGameUserWidget: SessionNameChanged %s."), *text.ToString());
	ViewModel->SetSessionName(text);
}

void UUDCreateGameUserWidget::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDCreateGameUserWidget: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}

void UUDCreateGameUserWidget::NewGame()
{
	UE_LOG(LogTemp, Log, TEXT("UUDCreateGameUserWidget: NewGame."));
	ViewModel->NewGame();
}