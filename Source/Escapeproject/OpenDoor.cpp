// Copyright Matthew Cross 2018

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "GameframeWork/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Containers/Array.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	
	Owner = GetOwner();
		

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 30.f) //TODO make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	//check if it's time to close the door

	if (((GetWorld()->GetTimeSeconds()) - LastDoorOpenTime) > DoorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*> OverLappingActors;
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	//iterate through them adding their masses
	for (const auto* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Item Name %s"), *Actor->GetName())
	}

	return TotalMass;
}