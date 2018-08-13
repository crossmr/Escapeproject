// Copyright Matthew Cross 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Delegates/Delegate.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);



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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BLUEPRINTASSIGNABLE)
		FDoorEvent OnOpen;
	UPROPERTY(BLUEPRINTASSIGNABLE)
		FDoorEvent OnClose;


private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	//UPROPERTY(EditAnywhere)
	AActor* Owner = nullptr; //owning door

	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	//returnst total mass in KG
	float GetTotalMassOfActorsOnPlate();


};
