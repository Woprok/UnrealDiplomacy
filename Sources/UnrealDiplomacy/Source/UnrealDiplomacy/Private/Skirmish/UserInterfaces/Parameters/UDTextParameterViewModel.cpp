// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "TextParameter"

void UUDTextParameterViewModel::Initialize()
{

}

void UUDTextParameterViewModel::Update()
{
	SetTextTitleText(FText::FromString(Content.Name));
	SetToolTipText(FText::FromString(Content.ToolTip));
	// Basically if user is editing the value, we will ignore it during the update.
	if (IsTextEditing)
	{
		return;
	}
	// TODO reset for text.
}

#undef LOCTEXT_NAMESPACE

void UUDTextParameterViewModel::SetContent(FUDTextParameter content)
{
	Content = content;
}

FString UUDTextParameterViewModel::GetAsText()
{
	return GetSelectedText().ToString();
}

void UUDTextParameterViewModel::StartTextEditation(const FText& InText)
{
	IsTextEditing = true;
}

void UUDTextParameterViewModel::StopTextEditation(const FText& InText, ETextCommit::Type CommitMethod)
{
	IsTextEditing = false;
	// Update only if value was changed.
	FText newText = InText;
	FText oldText = GetSelectedText();
	// TODO find source of the bug that causes the binding for EditableTextBox to not update automatically.
	if (!oldText.EqualTo(newText))
	{
		SetSelectedText(newText);
		OnChangeEvent.Broadcast();
	}
}

void UUDTextParameterViewModel::SetTextTitleText(FText newTextTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(TextTitleText, newTextTitleText);
}

FText UUDTextParameterViewModel::GetTextTitleText() const
{
	return TextTitleText;
}

void UUDTextParameterViewModel::SetToolTipText(FText newToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ToolTipText, newToolTipText);
}

FText UUDTextParameterViewModel::GetToolTipText() const
{
	return ToolTipText;
}

void UUDTextParameterViewModel::SetSelectedText(FText newSelectedText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedText, newSelectedText);
}

FText UUDTextParameterViewModel::GetSelectedText() const
{
	return SelectedText;
}