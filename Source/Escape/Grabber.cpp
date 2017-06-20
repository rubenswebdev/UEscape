// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("OK NOVO PAWN"))

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* Player = GetWorld()->GetFirstPlayerController();

	FVector PLocation;
	FRotator PRotator;

	Player->GetPlayerViewPoint(PLocation, PRotator);

	//pego minha localizacao somo com o vetor da minha rotacao para que aponte a linha para o lado que estiver olhando e multiplico pelo alcance
	FVector LineTraceEnd = PLocation + PRotator.Vector() * Reach;

	//Desenha a linha para debug apenas, pode ser removido depois
	DrawDebugLine(GetWorld(), PLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 1.f);


	//tag do objeto, usar colisao complexa ou nao, ignorar a si mesmo para nao detectar colisoes com o proprio objeto
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//com quem deve colidir, nesse caso todos os objetos do canal Corpo fisico
	FCollisionObjectQueryParams ColideWith(ECollisionChannel::ECC_PhysicsBody);
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		Hit, //salva nessa var o que colidiu
		PLocation, //define o ponto inicial da linha de colisao
		LineTraceEnd, //define o final da linha de colisao
		ColideWith, //define com o que vai colidir
		TraceParams //como deve colidir e quem ignorar
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		FString ColidiuCom = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Colidiu com: %s"), *ColidiuCom)
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Colidiu com nada ainda."))
	}


	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s - Rotation: %s - Vetor da rotacao: %s"), 
		*PLocation.ToString(), 
		*PRotator.ToString(), 
		*PRotator.Vector().ToString()
	)*/
}

