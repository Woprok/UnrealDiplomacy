// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Core/UserInterfaces/UDViewModel.h"

void UUDViewModelManager::RegisterViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: ViewModel(%d) successfully registered."));
	ViewModels.Add(viewModel);
}

void UUDViewModelManager::UpdateViewModels()
{
	for (auto viewModel : ViewModels)
	{
		viewModel->Update();
	}
}


TObjectPtr<UUDViewModel> UUDViewModelManager::Get(FName name)
{
	return ViewModels[0];
}