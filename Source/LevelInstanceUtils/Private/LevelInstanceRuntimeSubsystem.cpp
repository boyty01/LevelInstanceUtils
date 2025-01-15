// Copyright - DMTesseracT


#include "LevelInstanceRuntimeSubsystem.h"
#include "Actor/SubLevelActorManagerBase.h"
#include "ActorComponent/SubLevelActorScriptComponent.h"
#include "Object/SubLevelActorScriptBase.h"

void ULevelInstanceRuntimeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{

}

bool ULevelInstanceRuntimeSubsystem::RegisterManagerClass(const FName Name, ASubLevelActorManagerBase* Actor, const bool bOverrideExisting)
{
	if (!bOverrideExisting && Managers.Contains(Name)) return false;

	Managers.Add(Name, Actor);
	OnManagerAdded(Name, Actor);
	return true;
}

bool ULevelInstanceRuntimeSubsystem::GetManagerByName(const FName ManagerName, ASubLevelActorManagerBase*& Manager)
{

	bool exists = Managers.Contains(ManagerName);
	
	if(exists)
		Manager = *Managers.Find(ManagerName);
	
	return exists;
}

void ULevelInstanceRuntimeSubsystem::RequestScriptFromManager(const FLevelManagerClientRequest RequestData)
{
	// if the manager exists, dispatch immediately.
	if (Managers.Contains(RequestData.ManagerName))
	{
		ASubLevelActorManagerBase* Manager = *Managers.Find(RequestData.ManagerName);
		TSubclassOf<USubLevelActorScriptBase> ScriptClass;
		if (Manager->RequestScript(RequestData.ActorId, ScriptClass))
		{
			if (RequestData.ScriptComponent.IsValid())
				RequestData.ScriptComponent->InitScript(ScriptClass);
		}		
		return;
	}

	// if the manager doesnt exist, queue the request
	QueueRequest(RequestData);
}

void ULevelInstanceRuntimeSubsystem::OnManagerAdded(FName ManagerName, ASubLevelActorManagerBase* Manager)
{
	if (!Manager) return;

	// iterate and execute any queued requests for this manager.
	TArray<FLevelManagerClientRequest> ExistingRequests;
	QueuedRequests.MultiFind(ManagerName, ExistingRequests);

	for (auto& Request : ExistingRequests)
	{
		RequestScriptFromManager(Request);
	}

	// clear the associated keys from the queue.
	QueuedRequests.Remove(ManagerName);
}

void ULevelInstanceRuntimeSubsystem::QueueRequest(FLevelManagerClientRequest Request)
{
	QueuedRequests.Add(Request.ManagerName, Request);
}
