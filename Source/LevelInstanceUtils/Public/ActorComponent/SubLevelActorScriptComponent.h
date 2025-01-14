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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//The name of the manager actor that this component's owner should request its script from.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FName ManagerName;

	UFUNCTION()
	void SetupOwner();

	/*How long to wait before retrying a data query if the subsystem wasn't ready. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Settings")
	float RetryTime{0.5f};

	//How many times this component will retry a rejected data query before giving up.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Settings")
	int MaxRetries{3};

private:

	int NumRetries{ 0 };

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
