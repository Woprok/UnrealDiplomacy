// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDParameterEditorUserWidget.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"

void UUDParameterEditorUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDParameterEditorViewModel* parameterViewModel = CastChecked<UUDParameterEditorViewModel>(viewModel.Get());
	ViewModel = parameterViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}