// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PhysicsHandle == nullptr) { return; }
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab press"))
	auto HitResult = GetFirstPhysicsBodyInReach();
	if (HitResult.GetActor()) {
		auto ComponentToGrab = HitResult.GetComponent();
		if (PhysicsHandle == nullptr) { return; }
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab release"))
		PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {	
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()))
	}
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()))
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {	
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation,
		GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}

FVector UGrabber::GetLineTraceEnd() {
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}


