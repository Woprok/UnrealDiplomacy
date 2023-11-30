// Copyright Miroslav Valach


#include "Skirmish/UserInterfaces/UDModifierItemUserWidget.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Components/Image.h"

void UUDModifierItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDModifierItemViewModel* resourceItemViewModel = CastChecked<UUDModifierItemViewModel>(viewModel.Get());
	ViewModel = resourceItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}