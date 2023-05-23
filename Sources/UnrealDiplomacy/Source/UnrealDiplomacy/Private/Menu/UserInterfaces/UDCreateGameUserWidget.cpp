// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreateGameUserWidget.h"
#include "Menu/UserInterfaces/UDCreateGameViewModel.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDCreateGameUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDCreateGameViewModel* createGameViewModel = CastChecked<UUDCreateGameViewModel>(viewModel.Get());
	ViewModel = createGameViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDCreateGameUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDCreateGameViewModel::Back);
	NewGameButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDCreateGameViewModel::NewGame);
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