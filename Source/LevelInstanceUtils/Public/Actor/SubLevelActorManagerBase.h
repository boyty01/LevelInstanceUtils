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

	//For reference only. The name of the Level instance this actor is part of. Populated when using the editor macro.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Instance")
	FName OwningLevelInstance{FName("Unspecified")};

	//For Reference only. The FName of the associated actor. This is the shared name of the actor that is identical in every instance. Populated when using the editor macro.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Actor")
	FName ActorName{NAME_None};

	//A user friendly name to help designers identify this actor in the editor.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Friendly Name")
	FName FriendlyName{NAME_None};

	//The script class that should be handed to the key actor on request
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Script")
	TSubclassOf<USubLevelActorScriptBase> ScriptClass{nullptr};
};

UCLASS(Blueprintable)
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

	void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	/*Automatically create Map Keys from Guids in the specified Level Instance.Helper function to speed up populating this managers clients.
	This is the prefered method for populating clients and is necessary for previewing changes in editor using ApplyInEditor();
	*/
	UFUNCTION(CallInEditor, BlueprintCallable, Category ="Editor")
	void ClaimGuidsFromLevel();

	/* Apply the scripts at design time. This is for previewing only and script executions will not persist in map save state.
	   This will only apply to actors that were discovered using ClaimGuidsFromLevel.
	*/
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Editor")
	void ApplyInEditor();

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
