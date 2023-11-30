// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModelList.h"
#include "Core/UserInterfaces/UDViewModel.h"

FUDViewModelList::FUDViewModelList()
{

};

FUDViewModelList::FUDViewModelList(TArray<TObjectPtr<UUDViewModel>> viewModels) : ViewModels(viewModels)
{

};