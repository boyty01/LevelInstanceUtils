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


#include "ActorComponent/LevelInstanceActorScriptComponent.h"
#include "LevelInstanceRuntimeSubsystem.h"
#include "Actor/LevelInstanceActorManagerBase.h"
#include "Object/LevelInstanceActorScriptBase.h"
// Sets default values for this component's properties
ULevelInstanceActorScriptComponent::ULevelInstanceActorScriptComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void ULevelInstanceActorScriptComponent::InitScript(TSubclassOf<class ULevelInstanceActorScriptBase> ScriptClass)
{
	AActor* Owner = GetOwner();
	if (!ScriptClass || !Owner) return;

	// Just call the CDO rather than making a new instance.
	ULevelInstanceActorScriptBase* Script = ScriptClass.GetDefaultObject(); 
	if (Script)
	{
		Script->Execute(Owner);
	}
}

// Called when the game starts
void ULevelInstanceActorScriptComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	RequestScript();
}


void ULevelInstanceActorScriptComponent::RequestScript()
{

	if (!GetWorld()) return;
	ULevelInstanceRuntimeSubsystem* LISubsystem = GetWorld()->GetSubsystem<ULevelInstanceRuntimeSubsystem>();
	
	if (LISubsystem)
	{
		if (!GetOwner()) return;

		FGuid OwnerId = GetOwner()->GetActorInstanceGuid();
		FLevelManagerClientRequest Request = FLevelManagerClientRequest(ManagerName, this, OwnerId);
		LISubsystem->RequestScriptFromManager(Request);	
	}
}
