// Copyright Miroslav Valach


#include "Core/Simulation/UDSimulationBase.h"

void AUDSimulationBase::InitializeEnvironment(FUDSimulationEnvironment environment)
{
	overview.Height = environment.Height;
	overview.Width = environment.Width;
	overview.Seed = environment.Seed;
	overview.Step = -1;
}

void AUDSimulationBase::InitializePlayer(FUDSimulationPlayer player)
{
	overview.Players.Add(player);
}

void AUDSimulationBase::StartStep(FUDSimulationStep step)
{
	overview.Step = step.Step;
}

void AUDSimulationBase::EndStep(FUDSimulationStep step)
{
	overview.Step = step.Step;
}