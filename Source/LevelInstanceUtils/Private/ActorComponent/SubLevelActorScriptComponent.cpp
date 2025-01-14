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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USubLevelActorScriptComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetupOwner();
}


void USubLevelActorScriptComponent::SetupOwner()
{

	if (!GetWorld()) return;
	ULevelInstanceRuntimeSubsystem* LISubsystem = GetWorld()->GetSubsystem<ULevelInstanceRuntimeSubsystem>();
	
	if (LISubsystem)
	{

		// if the subsystem isn't ready, set a timer to try again, until max retries hit.
		if (!LISubsystem->IsReadyForClients())
		{
			if (NumRetries >= MaxRetries) return;

			NumRetries++;
			FTimerDelegate Delegate; 
			Delegate.BindUFunction(this, FName("SetupOwner"));
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, RetryTime, false);
		}

		ASubLevelActorManagerBase* Manager;
		if (!LISubsystem->GetManagerByName(ManagerName, Manager)) return;		
		AActor* Owner = GetOwner();
		if (Owner)
		{
			FGuid id = Owner->GetActorInstanceGuid();

			TSubclassOf<USubLevelActorScriptBase> ScriptClass;
			if (Manager->RequestScript(id, ScriptClass))
			{
				if (!ScriptClass) return;
				
				USubLevelActorScriptBase* Script = NewObject<USubLevelActorScriptBase>(Owner, ScriptClass);
				if (Script)
				{
					Script->Execute(Owner);
				}
			}

		}
	}
}

// Called every frame
void USubLevelActorScriptComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

