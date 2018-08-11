// Copyright Matthew Cross 2018

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "GameframeWork/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}



// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
		
	/// Look for attached physics handler
	
	///check if the physics handler is attached
	PhysicsHandleChecker();
	///check if the input component is attached
	CharacterInputChecker();
	
}

void UGrabber::CharacterInputChecker()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//if input component is attached
		UE_LOG(LogTemp, Warning, TEXT("Character Input Component found on %s"), *GetOwner()->GetName())
			///Bind input action
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
			InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		//if input component is not detected
		UE_LOG(LogTemp, Error, TEXT("Character Input Component not found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::PhysicsHandleChecker()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//if physics handle found

	}
	else
	{
		//if physics handle not found
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found on %s"), *(GetOwner()->GetName()))
	}
}



void  UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbed"))

	//LINE TRACE and see if we hit any actors with physics collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	//if we hit something then attach a physics handler
	if (ActorHit) {
		//Attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	};
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Released"))

		PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//If the physics handle is attached
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent) {
		//move the object that we're holding

		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}
		


	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player viewpoint every tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Set-up query parameters

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Line-Trace/Ray-cast out to reach distance

	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);


	//See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}
	return Hit;
}