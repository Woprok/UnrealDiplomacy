// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"

void UUDViewModelManager::SetModelManager(TObjectPtr<UUDActionAdministrator> model)
{
	Model = model;
}

void UUDViewModelManager::ForceInitialize()
{
	for (auto viewModel : ViewModels)
	{
		viewModel.Value->InitializeDefault();
	}
}

void UUDViewModelManager::ForceUpdate()
{
	for (auto viewModel : ViewModels)
	{
		viewModel.Value->FullUpdate();
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
	FUDViewModelCollection& existingCollectionViewModels = ViewModelCollections[name];
	for (int32 i = existingCollectionViewModels.Collection.Num(); i < desiredTotalCount; i++)
	{
		existingCollectionViewModels.Collection.Add(Create(viewModelType));
	}

	UE_LOG(LogTemp, Log, TEXT("UDViewModelManager: CollectionViewModel(%s) reached total %d."), *name.ToString(), desiredTotalCount);
}