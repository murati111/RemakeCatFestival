// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleComponent.h"

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
	if (Ramune!=nullptr)
	{
		Ramune->SetupAttachment(RootComponent);
		Ramune->SetRelativeScale3D(FVector(0.25f));
		Ramune->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Ramune_DM"));
	if (DestructibleComponent != nullptr)
	{
		DestructibleComponent->SetupAttachment(RootComponent);
		DestructibleComponent->SetRelativeScale3D(FVector(0.25f));
	}

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	if (BoxComp != nullptr)
	{
		BoxComp->SetupAttachment(DestructibleComponent);
		BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 505.0f));
		BoxComp->SetRelativeScale3D(FVector(4.25f, 4.25f, 15.5f));
	}

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObstacle::ReceiveDamageForObscale_Implementation(FVector HitNormalVector,bool IsDashing)
{	
	UE_LOG(LogTemp, Log, TEXT("ReceiveDamage"));
	if (IsDashing)
	{
		if (BreakObstacleSound != nullptr)
		{
			constexpr float Volume = 1.0f;
			UGameplayStatics::PlaySound2D(this, BreakObstacleSound, Volume);
		}
	}
	else
	{
		if (HitObstacleSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, HitObstacleSound);
		}
	}

	BoxComp->SetGenerateOverlapEvents(false);
	constexpr float LifeSpan = 2.0f;
	SetLifeSpan(LifeSpan);
}

