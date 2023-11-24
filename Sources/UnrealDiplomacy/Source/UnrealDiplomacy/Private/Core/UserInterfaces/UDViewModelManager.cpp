// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"

void UUDViewModelManager::SetModelManager(TObjectPtr<UUDActionAdministrator> model)
{
	Model = model;
}

bool UUDViewModelManager::RefreshViewModel(const FName& name)
{
	if (!ViewModels.Contains(name))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDViewModelManager: ViewModel(%s) is not registered."), *name.ToString());
		return false;
	}
	TObjectPtr<UUDViewModel> viewModel = ViewModels[name];
	viewModel->Refresh();
	UE_LOG(LogTemp, Log, TEXT("UUDViewModelManager: ViewModel(%s) is now refreshed."), *name.ToString());
	return true;
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
	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: Performing ViewModel(%s) setup."), *name.ToString());
	ViewModels[name]->Setup();
	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: ViewModel(%s) successfully registered."), *name.ToString());
}

TObjectPtr<UUDViewModel> UUDViewModelManager::Create(TSubclassOf<UUDViewModel> viewModelType)
{
	TObjectPtr<UUDViewModel> newViewModel = NewObject<UUDViewModel>(this, viewModelType);
	if (Model)
	{
		newViewModel->SetModel(Model);
		UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: ViewModel created with model reference."));
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: ViewModel created without model reference."));
	}
	return newViewModel;
}

TArray<TObjectPtr<UUDViewModel>>& UUDViewModelManager::GetCollection(const FName& name)
{
	return ViewModelCollections[name].Collection;
}

void UUDViewModelManager::RegisterCollection(FName name, TSubclassOf<UUDViewModel> viewModelType, int32 desiredTotalCount)
{
	if (!ViewModelCollections.Contains(name))
	{
		UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: CollectionViewModel(%s) is being created."), *name.ToString());
		ViewModelCollections.Add(name, FUDViewModelCollection(name));
	}

	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: CollectionViewModel(%s) is being expanded."), *name.ToString());
	for (int32 i = ViewModelCollections[name].Collection.Num(); i < desiredTotalCount; i++)
	{
		TObjectPtr<UUDViewModel> newViewModel = Create(viewModelType);
		ViewModelCollections[name].Collection.Add(newViewModel);
		// Modifying collections by Setup is possible, thus this must be called after we have created the member and placed it in.
		UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: Performing ViewModel(%s) setup."), *name.ToString());
		newViewModel->Setup();
	}
	// Basically if we ever end up with 0 after we had to create one, something is wrong and this project should fail as fast as possible.

	if (desiredTotalCount > 0)
		ensureMsgf(ViewModelCollections[name].Collection.Num() > 0, TEXT("UDViewModelManager: '%s' missing memebrs!"), *name.ToString());

	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: CollectionViewModel(%s) reached total %d."), *name.ToString(), desiredTotalCount);
}