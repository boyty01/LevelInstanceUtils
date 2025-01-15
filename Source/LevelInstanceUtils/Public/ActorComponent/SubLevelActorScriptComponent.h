// Copyright - DMTesseracT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SubLevelActorScriptComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEVELINSTANCEUTILS_API USubLevelActorScriptComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USubLevelActorScriptComponent();

	UFUNCTION(BlueprintCallable, Category="SubLevelManager")
	FName GetSubLevelManagerName() { return ManagerName; };

	void InitScript(TSubclassOf<class USubLevelActorScriptBase> ScriptClass);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//The name of the manager actor that this component's owner should request its script from.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FName ManagerName;

	UFUNCTION()
	void RequestScript();


private:

		
};
