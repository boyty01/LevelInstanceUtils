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
#include "GameFramework/Actor.h"
#include "Object/LevelInstanceActorScriptBase.h"
#include "LevelInstanceActorManagerBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LevelInstanceActorManager, Log, All);


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
	TSubclassOf<ULevelInstanceActorScriptBase> ScriptClass{nullptr};
};

UCLASS(Blueprintable)
class LEVELINSTANCEUTILS_API ALevelInstanceActorManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelInstanceActorManagerBase();

	UFUNCTION(BlueprintCallable, Category ="LevelInstanceManager")
	bool RequestScript(FGuid Guid, TSubclassOf<ULevelInstanceActorScriptBase>& ScriptClass)
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
