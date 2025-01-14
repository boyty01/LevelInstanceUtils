// Copyright - DMTesseracT

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubLevelActorScriptBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LEVELINSTANCEUTILS_API USubLevelActorScriptBase : public UObject
{
	GENERATED_BODY()
	
public:

	//execute the script
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="ActorScript")
	void Execute(AActor* Target);
};
