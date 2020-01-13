// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASuperItem::ASuperItem()
{
	USceneComponent* NewSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	if (NewSceneComponent != nullptr)
	{
		RootComponent = NewSceneComponent;
	}

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshObj(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMeshObj.Succeeded())
	{
		Plane->SetStaticMesh(PlaneMeshObj.Object);
		Plane->SetupAttachment(RootComponent);
	}
	Plane->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
	Plane->SetRelativeScale3D(FVector(1.75f, 1.75f, 1.75f));
	Plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComp->SetupAttachment(Plane);
	BoxComp->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.25f));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void ASuperItem::OnConstruction(const FTransform& Transform)
{
	Plane->SetMaterial(0, PlaneMaterial);

}

void ASuperItem::ReceiveDamage_Implementation(float A,int32& point)
{
	point = DashPoint;
	Destroy();
}

// Called when the game starts or when spawned
void ASuperItem::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle planeRotationTimerHandle;
	GetWorldTimerManager().SetTimer(planeRotationTimerHandle, this, &ASuperItem::PlaneRotation, 0.1f, true);
}


void ASuperItem::PlaneRotation()
{
	AddActorWorldRotation(FRotator(0.0f, 30.0f, 0.0f));
}

// Called every frame
void ASuperItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


