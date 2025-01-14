// Copyright - DMTesseracT


#include "LevelInstanceRuntimeSubsystem.h"
#include "Actor/SubLevelActorManagerBase.h"
void ULevelInstanceRuntimeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{

	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]() {
		bReadyForQueries = true; 
		});

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, delegate, 1.0f, false);
}
bool ULevelInstanceRuntimeSubsystem::RegisterManagerClass(const FName Name, ASubLevelActorManagerBase* Actor, const bool bOverrideExisting)
{
	if (!bOverrideExisting && Managers.Contains(Name)) return false;

	Managers.Add(Name, Actor);
	return true;
}

bool ULevelInstanceRuntimeSubsystem::GetManagerByName(const FName ManagerName, ASubLevelActorManagerBase*& Manager)
{
	// safe guard against calls that circumvent the standard process.
	if (!bReadyForQueries) return false;

	bool exists = Managers.Contains(ManagerName);
	
	if(exists)
		Manager = *Managers.Find(ManagerName);
	
	return exists;
}
