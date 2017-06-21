// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() { PrimaryComponentTick.bCanEverTick = true; }

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	ConfigManipuladorFisica();
	ConfigControle();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TestGrabAndMove();
}

void UGrabber::ConfigManipuladorFisica() {
	ManipuladorFisica = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!ManipuladorFisica) {
		UE_LOG(LogTemp, Error, TEXT("MANIPULADOR DE FISICA NAO ENCONTRADO %s"), *GetOwner()->GetName())
	}
}

void UGrabber::ConfigControle() {
	Controle = GetOwner()->FindComponentByClass<UInputComponent>();

	if (Controle) {
		Controle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Controle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Controle NAO ENCONTRADO %s"), *GetOwner()->GetName())
	}
}

FHitResult UGrabber::OnHit() {
	//Desenha a linha para debug apenas, pode ser removido depois
	//DrawDebugLine(GetWorld(), PLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 1.f);

	//tag do objeto, usar colisao complexa ou nao, ignorar a si mesmo para nao detectar colisoes com o proprio objeto
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//com quem deve colidir, nesse caso todos os objetos do canal Corpo fisico
	FCollisionObjectQueryParams ColideWith(ECollisionChannel::ECC_PhysicsBody);

	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		Hit, //salva nessa var o que colidiu
		GetStartLine(), //define o ponto inicial da linha de colisao
		GetEndLine(), //define o final da linha de colisao
		ColideWith, //define com o que vai colidir
		TraceParams //como deve colidir e quem ignorar
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		FString ColidiuCom = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Colidiu com: %s"), *ColidiuCom)
	}

	return Hit;
}

void UGrabber::TestGrabAndMove() {
	if (ManipuladorFisica->GrabbedComponent) {
		ManipuladorFisica->SetTargetLocationAndRotation(GetEndLine(), GetRotationLine());
	}
}

FVector UGrabber::GetStartLine() {
	APlayerController* Player = GetWorld()->GetFirstPlayerController();

	FVector PLocation;
	FRotator PRotator;
	Player->GetPlayerViewPoint(PLocation, PRotator);
	FVector LineTraceEnd = PLocation + PRotator.Vector() * Reach;

	return PLocation;
}

FVector UGrabber::GetEndLine() {
	APlayerController* Player = GetWorld()->GetFirstPlayerController();

	FVector PLocation;
	FRotator PRotator;
	Player->GetPlayerViewPoint(PLocation, PRotator);

	//pego minha localizacao somo com o vetor da minha rotacao para que aponte a linha para o lado que estiver olhando e multiplico pelo alcance
	FVector LineTraceEnd = PLocation + PRotator.Vector() * Reach;

	return LineTraceEnd;
}

FRotator UGrabber::GetRotationLine() {
	APlayerController* Player = GetWorld()->GetFirstPlayerController();

	FVector PLocation;
	FRotator PRotator;
	Player->GetPlayerViewPoint(PLocation, PRotator);
	FVector LineTraceEnd = PLocation + PRotator.Vector() * Reach;

	return PRotator;
}



void UGrabber::Grab() {
	auto Colisao = OnHit();
	auto ComponentToGrab = Colisao.GetComponent();
	auto Ator = Colisao.GetActor();
		
	if (Ator) {
		ManipuladorFisica->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, 
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);

		UE_LOG(LogTemp, Warning, TEXT("MOUSE DIREITO CLICADO, GRAB"))
	}
}


void UGrabber::Release() {

	if (ManipuladorFisica->GrabbedComponent) {
		ManipuladorFisica->ReleaseComponent();
	}

	UE_LOG(LogTemp, Warning, TEXT("MOUSE DIREITO SOLTADO, RELEASE"))
}



