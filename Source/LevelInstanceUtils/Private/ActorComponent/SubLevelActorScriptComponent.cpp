// Copyright - DMTesseracT


#include "ActorComponent/SubLevelActorScriptComponent.h"
#include "LevelInstanceRuntimeSubsystem.h"
#include "Actor/SubLevelActorManagerBase.h"
#include "Object/SubLevelActorScriptBase.h"
// Sets default values for this component's properties
USubLevelActorScriptComponent::USubLevelActorScriptComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void USubLevelActorScriptComponent::InitScript(TSubclassOf<class USubLevelActorScriptBase> ScriptClass)
{
	AActor* Owner = GetOwner();
	if (!ScriptClass || !Owner) return;

	// Just call the CDO rather than making a new instance.
	USubLevelActorScriptBase* Script = ScriptClass.GetDefaultObject(); //NewObject<USubLevelActorScriptBase>(Owner, ScriptClass);
	if (Script)
	{
		Script->Execute(Owner);
	}
}

// Called when the game starts
void USubLevelActorScriptComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	RequestScript();
}


void USubLevelActorScriptComponent::RequestScript()
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
