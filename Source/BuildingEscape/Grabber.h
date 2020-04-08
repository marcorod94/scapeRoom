// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f;
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	class UPhysicsHandleComponent* PhysicsHandle = nullptr;
	class UInputComponent* InputComponent = nullptr;

	// Ray-cast and grabs what's in reach
	void Grab();
	//Call when grab is released
	void Release();
	//Find (assumed) attached phyisc handle
	void FindPhysicsHandleComponent();
	//Setup (assumed) attached input component
	void SetupInputComponent();
	//Return hit for physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetLineTraceEnd();

};
