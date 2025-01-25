// Copyright (c) 2025 DMTesseracT Ltd
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "Actor/LevelInstanceActorManagerBase.h"
#include "LevelInstanceRuntimeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "ActorComponent/LevelInstanceActorScriptComponent.h"

DEFINE_LOG_CATEGORY(LevelInstanceActorManager);

// Sets default values
ALevelInstanceActorManagerBase::ALevelInstanceActorManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsSpatiallyLoaded = false;

}

void ALevelInstanceActorManagerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


#if WITH_EDITOR
void ALevelInstanceActorManagerBase::ClaimGuidsFromLevel()
{
	if (!LevelInstanceToQuery || !LevelInstanceToQuery->GetLoadedLevel()) return;

	ULevel* Level = LevelInstanceToQuery->GetLoadedLevel();
	for (auto& LActor : Level->Actors)
	{
		if (!LActor) continue;
		// only actors that have the script component are qualified
		ULevelInstanceActorScriptComponent* ScriptComponent = LActor->GetComponentByClass<ULevelInstanceActorScriptComponent>();
		if (ScriptComponent)
		{
			// ignore if appropriate.
			if (ScriptComponent->GetLevelInstanceManagerName() != ManagerName) continue;

			FGuid id = LActor->GetActorInstanceGuid();
			FLevelInstanceManagerClientData data;
			data.OwningLevelInstance = FName(LevelInstanceToQuery->GetActorLabel());
			data.ActorName = FName(LActor->GetActorLabel());
			if (GuidScripts.Contains(id))
			{
				FLevelInstanceManagerClientData& Existing = *GuidScripts.Find(id);
				data.FriendlyName = Existing.FriendlyName;
				data.ScriptClass = Existing.ScriptClass;				
			}
				

			GuidScripts.Add(id,data);
			MarkPackageDirty();
		}
	}
}
void ALevelInstanceActorManagerBase::ApplyInEditor()
{
	// Find the appropriate actors and apply the scripts. This is for design time consistency so designers can see the effect without
	// having to play the level. These scripts will still need to be executed at runtime for changes to persist.

	if (!GetWorld()) return;
	TArray<AActor*> LevelInstances;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelInstance::StaticClass(), LevelInstances);
	for (auto& Pair : GuidScripts)
	{
		for (auto& Instance : LevelInstances)
		{
			ALevelInstance* AsInstance = Cast<ALevelInstance>(Instance);
			if (!AsInstance || !AsInstance->GetLoadedLevel()) continue;
			for (auto& Actor : AsInstance->GetLoadedLevel()->Actors)
			{
				if (Actor->GetActorLabel().Equals(Pair.Value.ActorName.ToString()) && AsInstance->GetActorLabel().Equals(Pair.Value.OwningLevelInstance.ToString()))
				{
					ULevelInstanceActorScriptComponent* ScriptComp = Actor->GetComponentByClass<ULevelInstanceActorScriptComponent>();
					if (!ScriptComp) continue;

					ScriptComp->InitScript(Pair.Value.ScriptClass);
				}
			}
		}
	}

}
#endif

// Called when the game starts or when spawned
void ALevelInstanceActorManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
	ULevelInstanceRuntimeSubsystem* LISubsystem = GetWorld()->GetSubsystem<ULevelInstanceRuntimeSubsystem>();
	if (LISubsystem)
	{
		if (LISubsystem->RegisterManagerClass(ManagerName, this))
		{
			UE_LOG(LevelInstanceActorManager, Log, TEXT("%s Manager successfuly registered."), *ManagerName.ToString());
		}
	}
}


