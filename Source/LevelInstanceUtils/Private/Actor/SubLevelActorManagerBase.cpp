// Copyright - DMTesseracT


#include "Actor/SubLevelActorManagerBase.h"
#include "LevelInstanceRuntimeSubsystem.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ActorComponent/SubLevelActorScriptComponent.h"

DEFINE_LOG_CATEGORY(SubLevelActorManager);

// Sets default values
ASubLevelActorManagerBase::ASubLevelActorManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsSpatiallyLoaded = false;

}

#if WITH_EDITOR
void ASubLevelActorManagerBase::ClaimGuidsFromlevel()
{
	if (!LevelInstanceToQuery || !LevelInstanceToQuery->GetLoadedLevel()) return;

	ULevel* Level = LevelInstanceToQuery->GetLoadedLevel();
	for (auto& LActor : Level->Actors)
	{
		if (!LActor) continue;
		// only actors that have the script component are qualified
		USubLevelActorScriptComponent* ScriptComponent = LActor->GetComponentByClass<USubLevelActorScriptComponent>();
		if (ScriptComponent)
		{
			FGuid id = LActor->GetActorInstanceGuid();
			FLevelInstanceManagerClientData data;
			data.OwningLevelInstance = FName(LevelInstanceToQuery->GetActorLabel());
			data.ActorName = FName(LActor->GetActorLabel());
			data.ScriptClass;
			if (!GuidScripts.Contains(id)) GuidScripts.Add(id,data); // dont overwrite it if its already there.
		}
	}
}
#endif

// Called when the game starts or when spawned
void ASubLevelActorManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
	ULevelInstanceRuntimeSubsystem* LISubsystem = GetWorld()->GetSubsystem<ULevelInstanceRuntimeSubsystem>();
	if (LISubsystem)
	{
		if (LISubsystem->RegisterManagerClass(ManagerName, this))
		{
			UE_LOG(SubLevelActorManager, Log, TEXT("%s Manager successfuly registered."), *ManagerName.ToString());
		}
	}
}


