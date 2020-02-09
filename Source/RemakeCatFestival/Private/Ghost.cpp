// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Public/MainGameModeBase.h"

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
	GetWorldTimerManager().SetTimer(LoadingTimeHandle, this, &AGhost::LoadingGhostData, GameMode->RecordingDeltaTime, true);
	SetActorLocation(GameInstance->LoadingGhostData.Positions[count]);
}

void AGhost::StopLoadingGhost()
{
	GetWorldTimerManager().ClearTimer(LoadingTimeHandle);
}
void AGhost::LoadingGhostData()
{
	count++;
	SetActorLocation(GameInstance->LoadingGhostData.Positions[count]);
	Speed = GameInstance->LoadingGhostData.Speeds[count];
	//UE_LOG(LogTemp, Error, TEXT("%s"), (GameInstance->LoadingGhostData.IsStops[count] ? TEXT("True") :TEXT("false")) );
	if (GameInstance->LoadingGhostData.IsStops[count])
	{
		//UE_LOG(LogTemp, Log, TEXT("True"));
		PlayAnimMontage(DamageAnimation);
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("false"));
	}
	if (GameInstance->LoadingGhostData.Positions.Max()-1 <= count)
	{
		StopLoadingGhost();
	}
}
// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	//GameInstance->LoadingGhostData = GameMode->LoadGhostRecord(0);
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

