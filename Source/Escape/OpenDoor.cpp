// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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
	AtorAtivaPorta = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	if (!TaAberto) {
		AActor *Actor = GetOwner();

		Actor->SetActorRotation(FRotator(0.f, Actor->GetActorRotation().Yaw - DoorAngle, 0.f));

		TaAberto = true;
		lastTimeOpen = GetWorld()->GetTimeSeconds();
	}
}

void UOpenDoor::CloseDoor() {

	AActor *Actor = GetOwner();
	FRotator Rot = Actor->GetActorRotation();

	FRotator NewRotator = FRotator(0.f, Rot.Yaw + DoorAngle, 0.f);
	Actor->SetActorRotation(NewRotator);

	TaAberto = false;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(AtorAtivaPorta)) {
		OpenDoor();
	}

	if (!PressurePlate->IsOverlappingActor(AtorAtivaPorta) && GetWorld()->GetTimeSeconds() - lastTimeOpen >= secondsToClose && TaAberto) {
		CloseDoor();
	}
}

