// Copyright Miroslav Valach

#include "Core/UserInterfaces/Components/UDListView.h"
#include "Core/UserInterfaces/UDViewModelList.h"

FUDViewModelList UUDListView::GetViewModelList() const
{
	return ViewModelList;
}

void UUDListView::SetViewModelList(const FUDViewModelList& collection)
{
	ViewModelList = collection;
	SetListItems(collection.ViewModels);
}