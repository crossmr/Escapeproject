// Copyright Matthew Cross 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Delegates/Delegate.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEPROJECT_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BLUEPRINTASSIGNABLE)
		FOnOpenRequest OnOpenRequest;


private:
	UPROPERTY(VisibleAnywhere)	
	float OpenAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	//UPROPERTY(EditAnywhere)
	AActor* Owner = nullptr; //owning door

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	float LastDoorOpenTime;

	//returnst total mass in KG
	float GetTotalMassOfActorsOnPlate();


};
