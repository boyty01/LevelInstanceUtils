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

/*Stored data for instances where a script component requests data before a manager is registered. Stores a ref to 
  the component that requested it and the actor Id the data is for. 
*/
USTRUCT(BlueprintType)
struct FLevelManagerClientRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FName ManagerName{NAME_None};

	UPROPERTY()
	TWeakObjectPtr<class USubLevelActorScriptComponent> ScriptComponent{ nullptr };

	UPROPERTY()
	FGuid ActorId;

	FLevelManagerClientRequest() {};

	FLevelManagerClientRequest(FName managerName, TWeakObjectPtr<class USubLevelActorScriptComponent> scriptComponent, FGuid actorId)
	{
		ManagerName = managerName;
		ScriptComponent = scriptComponent;
		ActorId = actorId;
	}
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
	

	/*Handles getting script data for a client component as soon as the manager is available. if the manager is available, it will
	call back immediately, if not it will queue the request until the manager has registered. */
	UFUNCTION(BlueprintCallable, Category ="LevelInstanceSubsystem")
	void RequestScriptFromManager(const FLevelManagerClientRequest RequestData);

private:

	void OnManagerAdded(FName ManagerName, ASubLevelActorManagerBase* Manager);

	void QueueRequest(FLevelManagerClientRequest Request);

	void DispatchScriptRequest();

	// stores any queued requests for a manager that may have been received before the manager is ready.
	TMultiMap<FName, FLevelManagerClientRequest> QueuedRequests;


	TMap<FName, ASubLevelActorManagerBase*> Managers;
};
