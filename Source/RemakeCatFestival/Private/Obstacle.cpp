// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TArray<FName> tagsTmp;
	tagsTmp.Add(ActorTag);
	Tags = tagsTmp;

	//SceneComponentÇRootÇ…ê›íË
	USceneComponent* NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	if (NewRootComponent != nullptr)
	{
		RootComponent = NewRootComponent;
	}

	Ramune = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ramune"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RamuneMeshObj(TEXT("/Game/Cat/Objects/Ramune/SM_Ramune.SM_Ramune"));
	if (RamuneMeshObj.Succeeded())
	{
		Ramune->SetStaticMesh(RamuneMeshObj.Object);
		Ramune->SetupAttachment(RootComponent);
		Ramune->SetRelativeScale3D(FVector(0.25f));
		Ramune->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(Ramune);
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 505.0f));
	BoxComp->SetRelativeScale3D(FVector(4.25f, 4.25f, 15.5f));
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::ReceiveDamage_Implementation(int32& point)
{
	UE_LOG(LogTemp, Log, TEXT("MyIntValue"));
	point = 0;
	if (HitObstacleSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, HitObstacleSound);
	}
	Destroy();
}

