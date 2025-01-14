// Copyright - DMTesseracT

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelInstanceRuntimeSubsystem.generated.h"


USTRUCT()
struct FLevelInstanceData
{
	GENERATED_BODY();


};
/**
 * 
 */
UCLASS()
class LEVELINSTANCEUTILS_API ULevelInstanceRuntimeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void OnWorldBeginPlay(UWorld& InWorld )override;

	// Register an valid actor with the current world, under the specified name. bOverrideExisting will replace any managers with the same name.
	UFUNCTION(BlueprintCallable, Category ="LevelInstanceSubsystem")
	bool RegisterManagerClass(const FName Name, ASubLevelActorManagerBase* Actor, const bool bOverrideExisting = true);

	// Get the manager class by its registered name.
	UFUNCTION(BlueprintCallable, Category="LevelInstanceSubsystem")
	bool GetManagerByName(const FName ManagerName, ASubLevelActorManagerBase*& Manager);
	
	UFUNCTION(BlueprintCallable, Category = "LevelInstanceSubsystem")
	bool IsReadyForClients() { return bReadyForQueries; };

private:

	void ReleaseQueryLock() {
		bReadyForQueries = true;
	};

	/*Forces a slight delay on any actors requesting data to ensure managers get time to register 
	before components start grabbing data when the world loads.
	*/
	bool bReadyForQueries{ false };

	TMap<FName, ASubLevelActorManagerBase*> Managers;
};
