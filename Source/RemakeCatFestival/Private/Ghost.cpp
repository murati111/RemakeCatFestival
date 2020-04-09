// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Public/MainGameModeBase.h"

AMainGameModeBase* AGhost::GetGameMode()
{
	return  Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode()) ? Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

void AGhost::BeginPlay()
{
	LoadingGhostCount = 0;
	Super::BeginPlay();
}

// Sets default values
AGhost::AGhost()
{
 	//Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(80.0f, 100.0f);

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> GhostMeshObj(TEXT("/Game/Cat/Charcter/SK_Cat.SK_Cat"));
	//if (GhostMeshObj.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(GhostMeshObj.Object);
	//	static ConstructorHelpers::FClassFinder<UAnimInstance> GhostAnimBPClass(TEXT("/Game/Cat/Animation/ABP_GhostCat"));

	//	if (GhostAnimBPClass.Class)
	//	{
	//		GetMesh()->SetAnimClass(GhostAnimBPClass.Class);
	//	}
	//	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageObj(TEXT("/Game/Cat/Animation/AM_Damage.AM_Damage"));
	//	if (AnimMontageObj.Succeeded())
	//	{
	//		DamageAnimation = AnimMontageObj.Object;
	//	}

	//}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageObj(TEXT("/Game/Cat/Animation/AM_Damage.AM_Damage"));
	if (AnimMontageObj.Succeeded())
	{
		DamageAnimation = AnimMontageObj.Object;
	}
	GetMesh()->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GameInstance = UMainGameInstance::GetInstance();
}

void AGhost::StartLoadingGhost()
{
	if (GetGameMode() == nullptr) { return; }
	GetWorldTimerManager().SetTimer(LoadingTimeHandle, this, &AGhost::LoadingGhostData, GetGameMode()->GetRecordingDeltaTime(), true);
	SetActorLocation(GameInstance->GetLoadingGhostData().Positions[LoadingGhostCount]);
}

void AGhost::StopLoadingGhost()
{
	GetWorldTimerManager().ClearTimer(LoadingTimeHandle);
}
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AGhost::LoadingGhostData()
{
	if (GameInstance == nullptr) { return; }
	LoadingGhostCount++;	
	if (GameInstance->GetLoadingGhostData().Positions.Max() <= LoadingGhostCount)
	{
		StopLoadingGhost();
		return;
	}
	SetActorLocation(GameInstance->GetLoadingGhostData().Positions[LoadingGhostCount]);
	Speed = GameInstance->GetLoadingGhostData().Speeds[LoadingGhostCount];
	//UE_LOG(LogTemp, Error, TEXT("GhostSpeed %f"), Speed);
	if (GameInstance->GetLoadingGhostData().IsStops[LoadingGhostCount])
	{
		//UE_LOG(LogTemp, Log, TEXT("True"));
		PlayAnimMontage(DamageAnimation);
	}
}