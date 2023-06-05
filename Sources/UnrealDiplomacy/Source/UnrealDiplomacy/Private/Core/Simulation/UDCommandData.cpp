// Copyright Miroslav Valach

#include "Core/Simulation/UDCommandData.h"

FUDCommandData::FUDCommandData()
{

}

FUDCommandData::FUDCommandData(EUDCommandType command) : Command(command)
{

}

FString FUDCommandData::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	switch (Command)
	{
	case EUDCommandType::None:
		formatArgs.Add(TEXT("commnadName"), TEXT("None"));
		break;
	case EUDCommandType::StartGame:
		formatArgs.Add(TEXT("commnadName"), TEXT("Start Game"));
		break;
	case EUDCommandType::HostClosedGame:
		formatArgs.Add(TEXT("commnadName"), TEXT("Host Closed Game"));
		break;
	case EUDCommandType::PlayerLeftGame:
		formatArgs.Add(TEXT("commnadName"), TEXT("Player Left Game"));
		break;
	default:
		unimplemented();
		formatArgs.Add(TEXT("commnadName"), TEXT("ERROR"));
		break;
	}
	return FString::Format(TEXT("Command[{commnadName}]"), formatArgs);
}