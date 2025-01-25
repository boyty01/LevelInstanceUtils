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

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelInstanceRuntimeSubsystem.generated.h"


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
	TWeakObjectPtr<class ULevelInstanceActorScriptComponent> ScriptComponent{ nullptr };

	UPROPERTY()
	FGuid ActorId;

	FLevelManagerClientRequest() {};

	FLevelManagerClientRequest(FName managerName, TWeakObjectPtr<class ULevelInstanceActorScriptComponent> scriptComponent, FGuid actorId)
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
	bool RegisterManagerClass(const FName Name, ALevelInstanceActorManagerBase* Actor, const bool bOverrideExisting = true);


	// Get the manager class by its registered name.
	UFUNCTION(BlueprintCallable, Category="LevelInstanceSubsystem")
	bool GetManagerByName(const FName ManagerName, ALevelInstanceActorManagerBase*& Manager);
	

	/*Handles getting script data for a client component as soon as the manager is available. if the manager is available, it will
	call back immediately, if not it will queue the request until the manager has registered. */
	UFUNCTION(BlueprintCallable, Category ="LevelInstanceSubsystem")
	void RequestScriptFromManager(const FLevelManagerClientRequest RequestData);

private:

	void OnManagerAdded(FName ManagerName, ALevelInstanceActorManagerBase* Manager);

	void QueueRequest(FLevelManagerClientRequest Request);

	void DispatchScriptRequest();

	// stores any queued requests for a manager that may have been received before the manager is ready.
	TMultiMap<FName, FLevelManagerClientRequest> QueuedRequests;


	TMap<FName, ALevelInstanceActorManagerBase*> Managers;
};
