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


#include "LevelInstanceRuntimeSubsystem.h"
#include "Actor/LevelInstanceActorManagerBase.h"
#include "ActorComponent/LevelInstanceActorScriptComponent.h"
#include "Object/LevelInstanceActorScriptBase.h"

void ULevelInstanceRuntimeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{

}

bool ULevelInstanceRuntimeSubsystem::RegisterManagerClass(const FName Name, ALevelInstanceActorManagerBase* Actor, const bool bOverrideExisting)
{
	if (!bOverrideExisting && Managers.Contains(Name)) return false;

	Managers.Add(Name, Actor);
	OnManagerAdded(Name, Actor);
	return true;
}

bool ULevelInstanceRuntimeSubsystem::GetManagerByName(const FName ManagerName, ALevelInstanceActorManagerBase*& Manager)
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
		ALevelInstanceActorManagerBase* Manager = *Managers.Find(RequestData.ManagerName);
		TSubclassOf<ULevelInstanceActorScriptBase> ScriptClass;
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

void ULevelInstanceRuntimeSubsystem::OnManagerAdded(FName ManagerName, ALevelInstanceActorManagerBase* Manager)
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
