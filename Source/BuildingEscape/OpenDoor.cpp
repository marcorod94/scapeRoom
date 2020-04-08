// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (PressurePlate == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Missing pressure plate definition"))
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;
	TArray<AActor*> OverLappingActors;
	if (PressurePlate == nullptr) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);
	for (const AActor* Actor : OverLappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

