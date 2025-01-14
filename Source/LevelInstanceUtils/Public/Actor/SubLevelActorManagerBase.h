// Copyright - DMTesseracT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Object/SubLevelActorScriptBase.h"
#include "SubLevelActorManagerBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SubLevelActorManager, Log, All);


USTRUCT(BlueprintType)
struct FLevelInstanceManagerClientData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Actor")
	FName ActorName{NAME_None};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Script")
	TSubclassOf<USubLevelActorScriptBase> ScriptClass{nullptr};
};

UCLASS()
class LEVELINSTANCEUTILS_API ASubLevelActorManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubLevelActorManagerBase();

	UFUNCTION(BlueprintCallable, Category ="SublevelManager")
	bool RequestScript(FGuid Guid, TSubclassOf<USubLevelActorScriptBase>& ScriptClass)
	{
		bool exists = GuidScripts.Contains(Guid);
		if(exists) ScriptClass = *GuidScripts.Find(Guid)->ScriptClass;

		return exists;
	};

#if WITH_EDITOR
	//Automatically create Map Keys from Guids in the specified Level Instance. Helper function to speed up populating this managers clients.
	UFUNCTION(CallInEditor, BlueprintCallable, Category ="Editor")
	void ClaimGuidsFromlevel();
#endif

#if WITH_EDITORONLY_DATA
	//When set, ClaimGuidsFromLevel will use the specified Level Instance to build its FGuid Map.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Editor")
	class ALevelInstance* LevelInstanceToQuery;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The name to register this instance under. This is the name actors will use to lookup its manager in the Subsystem.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Manager Settings")
	FName ManagerName;

	// Map of scripts associated with an actor Instance Guid. Use the utility to get the appropriate Guid for an actor.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Actor Scripts")
	TMap<FGuid, FLevelInstanceManagerClientData> GuidScripts;

private:


};
