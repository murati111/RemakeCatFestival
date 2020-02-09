// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SceneComponentÇRootÇ…ê›íË
	NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	if (NewRootComponent != nullptr)
	{
		RootComponent = NewRootComponent;
	}
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 111.0f));
	BoxComp->SetRelativeScale3D(FVector(6.0f, 1.0f, 3.5f));
	TArray<FName> tagsTmp;
	tagsTmp.Add(ActorTag);
	Tags = tagsTmp;

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::ReceiveDamage_Implementation(int32& point)
{
	UGameplayStatics::PlaySound2D(this, GoalSE);
	point = 0;
}