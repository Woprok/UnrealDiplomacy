// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Core/UserInterfaces/UDViewModel.h"


void UUDViewModelManager::ForceUpdate()
{
	for (auto viewModel : ViewModels)
	{
		viewModel.Value->Update();
	}
}

TObjectPtr<UUDViewModel> UUDViewModelManager::Get(const FName& name)
{
	return ViewModels[name];
}

void UUDViewModelManager::Register(FName name, TSubclassOf<UUDViewModel> viewModelType)
{
	if (ViewModels.Contains(name))
	{
		UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: ViewModel(%s) is already registered."), *name.ToString());
		return;
	}

	ViewModels.Add(name, Create(viewModelType));
	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: ViewModel(%s) successfully registered."), *name.ToString());
}

TObjectPtr<UUDViewModel> UUDViewModelManager::Create(TSubclassOf<UUDViewModel> viewModelType)
{
	return NewObject<UUDViewModel>(this, viewModelType);
}