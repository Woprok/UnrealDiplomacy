// Copyright Miroslav Valach

#include "Core/UserInterfaces/Components/UDTileView.h"
#include "Core/UserInterfaces/UDViewModelList.h"

FUDViewModelList UUDTileView::GetViewModelList() const
{
	return ViewModelList;
}

void UUDTileView::SetViewModelList(const FUDViewModelList& collection)
{
	ViewModelList = collection;
	SetListItems(collection.ViewModels);
}