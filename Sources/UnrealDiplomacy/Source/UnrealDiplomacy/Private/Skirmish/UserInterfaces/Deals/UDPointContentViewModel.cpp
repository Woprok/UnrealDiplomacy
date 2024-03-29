// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyInvoker.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTextParameter.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyValueParameters.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "PointContent"

int32 UUDPointContentViewModel::UniqueNameDefinition = 0;

void UUDPointContentViewModel::Setup()
{
	if (!IsUniqueNameDefined)
	{
		DefineInstances();
		IsUniqueNameDefined = true;
	}
	FText point = FText(LOCTEXT("PointContent", "Point Content"));
	SetPointText(point);
	FText editor = FText(LOCTEXT("PointContent", "Edit"));
	SetEditorText(editor);
	FText ignored = FText(LOCTEXT("PointContent", "X"));
	SetIgnoredText(ignored);
	SetIsIgnoredValue(false);

	// Retrieve model
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> editorModel = hud->GetViewModelCollection(ParameterEditorInstanceName, ParameterEditorType);
	ParameterEditorInstance = Cast<UUDParameterEditorViewModel>(editorModel);
	SetParameterEditorContent(FUDViewModelContent(ParameterEditorInstance));
	// TODO if this is equal to previous version that called it each update. If yes, remove the clear part.
	ParameterEditorInstance->DealActionUpdated.Clear();
	ParameterEditorInstance->InvokerUpdated.Clear();
	ParameterEditorInstance->ValuesUpdated.Clear();
	ParameterEditorInstance->TextUpdated.Clear();
	ParameterEditorInstance->DealActionUpdated.AddUObject(this, &UUDPointContentViewModel::SaveDealActionChange);
	ParameterEditorInstance->InvokerUpdated.AddUObject(this, &UUDPointContentViewModel::SaveInvokerChange);
	ParameterEditorInstance->ValuesUpdated.AddUObject(this, &UUDPointContentViewModel::SaveValuesChange);
	ParameterEditorInstance->TextUpdated.AddUObject(this, &UUDPointContentViewModel::SaveTextChange);
}

void UUDPointContentViewModel::Refresh()
{
	if (!Content.IsIgnored)
	{
		FFormatOrderedArguments args;
		args.Add(FFormatArgumentValue(Content.PointId));
		args.Add(FFormatArgumentValue(FText::FromString(Content.PointTitle)));
		SetPointTitleText(FText::Format(LOCTEXT("PointContent", "Point {0}: {1}"), args));
	}
	else
	{
		SetPointTitleText(FText(LOCTEXT("PointContent", "Point Ignored")));
	}
	SetPointText(FText::FromString(Content.PointContent));
	SetIsIgnoredValue(Content.IsIgnored);
	UpdateEditor();
}

#undef LOCTEXT_NAMESPACE

void UUDPointContentViewModel::Ignore()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPointContentViewModel: Ignore point %d."), Content.PointId);
	Model->RequestAction(Model->GetAction(UUDDealActionPointIgnore::ActionTypeId,
		{
			Content.DealId, Content.PointId
		}
	));
}

void UUDPointContentViewModel::SetContent(FUDDealPointMinimalInfo content)
{
	Content = Model->GetPointInteraction(content.DealId, content.PointId);
}

void UUDPointContentViewModel::SaveDealActionChange(int32 actionId) 
{
	Model->RequestAction(Model->GetAction(UUDDealActionPointModifyAction::ActionTypeId, 
		{ 
			Content.DealId, Content.PointId, actionId 
		}
	));
}
void UUDPointContentViewModel::SaveInvokerChange(int32 invokerId)
{
	Model->RequestAction(Model->GetAction(UUDDealActionPointModifyInvoker::ActionTypeId,
		{
			Content.DealId, Content.PointId, invokerId
		}
	));
}
void UUDPointContentViewModel::SaveValuesChange(TArray<int32> values)
{
	TArray<int32> valueParameters = { };
	valueParameters.Add(Content.DealId);
	valueParameters.Add(Content.PointId);
	valueParameters.Append(values);

	Model->RequestAction(Model->GetAction(UUDDealActionPointModifyValueParameters::ActionTypeId, 
		valueParameters	
	));
}
void UUDPointContentViewModel::SaveTextChange(FString text)
{
	Model->RequestAction(Model->GetAction(UUDDealActionPointModifyTextParameter::ActionTypeId,
		{
			Content.DealId, Content.PointId
		}, 
		text
	));
}

void UUDPointContentViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPointContentViewModel: UpdateEditor."));
	ParameterEditorInstance->SetContent(Content.Parameters);
	ParameterEditorInstance->Refresh();
}

void UUDPointContentViewModel::DefineInstances()
{
	int32 uniqueId = UUDPointContentViewModel::UniqueNameDefinition++;
	ParameterEditorType = UUDParameterEditorViewModel::StaticClass();
	ParameterEditorInstanceName = FName(ParameterEditorInstanceName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDPointContentViewModel: Defined editor [%d]."), uniqueId);
}

void UUDPointContentViewModel::SetPointTitleText(FText newPointTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PointTitleText, newPointTitleText);
}

FText UUDPointContentViewModel::GetPointTitleText() const
{
	return PointTitleText;
}

void UUDPointContentViewModel::SetPointText(FText newPointText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PointText, newPointText);
}

FText UUDPointContentViewModel::GetPointText() const
{
	return PointText;
}

void UUDPointContentViewModel::SetEditorText(FText newEditorText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EditorText, newEditorText);
}

FText UUDPointContentViewModel::GetEditorText() const
{
	return EditorText;
}

void UUDPointContentViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDPointContentViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}

void UUDPointContentViewModel::SetParameterEditorContent(FUDViewModelContent newParameterEditorContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(ParameterEditorContent, newParameterEditorContent);
}

FUDViewModelContent UUDPointContentViewModel::GetParameterEditorContent() const
{
	return ParameterEditorContent;
}

void UUDPointContentViewModel::SetIgnoredText(FText newIgnoredText)
{
	UE_MVVM_SET_PROPERTY_VALUE(IgnoredText, newIgnoredText);
}

FText UUDPointContentViewModel::GetIgnoredText()  const
{
	return IgnoredText;
}

void UUDPointContentViewModel::SetIsIgnoredValue(bool newIsIgnoredValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsIgnoredValue, newIsIgnoredValue);
}

bool UUDPointContentViewModel::GetIsIgnoredValue()  const
{
	return IsIgnoredValue;
}
