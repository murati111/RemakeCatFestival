// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat.h"
#include "Public/CatInterface.h"
#include "Public/MainGameModeBase.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"


// Sets default values
ACat::ACat()
{

	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(80.0f, 100.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Cat/Charcter/SK_Cat.SK_Cat"));
	if (PlayerMeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerMeshObj.Object);
		static ConstructorHelpers::FClassFinder<UAnimInstance> PlayerAnimBPClass(TEXT("/Game/Cat/Animation/ABP_Cat"));
		
		if (PlayerAnimBPClass.Class)
		{
			GetMesh()->SetAnimClass(PlayerAnimBPClass.Class);
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageAnimMontageObj(TEXT("/Game/Cat/Animation/AM_Damage.AM_Damage"));
		if (DamageAnimMontageObj.Succeeded())
		{
			DamageAnimation = DamageAnimMontageObj.Object;
		}

	}
	GetMesh()->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	CatSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CatSpringArm"));
	CatSpringArm->SetupAttachment(GetCapsuleComponent());

	CatCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CatCameraComponent"));
	CatCameraComponent->SetupAttachment(CatSpringArm);

	CatSpringArm->SetRelativeLocation(FVector(80.0f, 0.0f, 50.0f));
	CatCameraComponent->SetRelativeLocation(FVector(-90, 0, 100));
	CatCameraComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MeshCollision"));
	BoxComp->SetupAttachment(GetMesh());
	BoxComp->SetRelativeLocation(FVector(0.0f, -40.0f, 320.0f));
	BoxComp->SetRelativeScale3D(FVector(3.0f, 10.0f, 5.5f));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACat::OnOverlapBegin);

	EscapeFlipFloop = true;
	bIsEscaping = false;
	bIsDamaging = false;
	bIsHitObscle = false;

}

// Called when the game starts or when spawned
void ACat::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	PlayerController = GetWorld()->GetFirstPlayerController();
	SetMaxSpeedAndAccel(DefaultMaxSpeed, DefaultMaxAcceleration);
	UE_LOG(LogTemp, Log, TEXT("CatBegin"));
}

// Called every frame
void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	//MoveForwardの入力が来たらMoveForward関数を呼び出す
	PlayerInputComponent->BindAxis("MoveForward", this, &ACat::MoveForward);
	PlayerInputComponent->BindAction("Escape", IE_Pressed,this, &ACat::EscapeTwoWays);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACat::Dash);

}


void ACat::MoveForward(float Val)
{
	if (Val > 0)
	{
		//if (bIsDamaging) { return; }
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ACat::EscapeTwoWays()
{
	if (bIsEscaping) { return; }
	//if (bIsDamaging) { return; }

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("EscapeTwoWaysMoving"), EscapeFlipFloop);
	GetWorldTimerManager().SetTimer(EscapeTimerHandle, TimerDelegate, 0.001f, true);
	//EscapeTwoWaysMoving(EscapeFlipFloop);
	if (EscapeSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, EscapeSound);
	}
	EscapeFlipFloop = !EscapeFlipFloop;
}

void ACat::EscapeTwoWaysMoving(bool IsRight)
{
	constexpr float OffsetPerTime = 1.0f;
	float ReverseScale = 1.0f;
	if (IsRight)
	{
		ReverseScale = -1.0f;
	}
	else
	{
		ReverseScale = 1.0f;
	}
	if (EscapeOffset < EscapeLength)
	{
		bIsEscaping = true;
		AddActorLocalOffset(FVector(0.f, ReverseScale * OffsetPerTime,0.f));
		EscapeOffset += OffsetPerTime;
	}
	else
	{
		bIsEscaping = false;
		GetWorldTimerManager().ClearTimer(EscapeTimerHandle);
		EscapeOffset = 0.0f;
	}
}

void ACat::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	int32 dp = 0;
	bool bIsImplemted = OtherActor->GetClass()->ImplementsInterface(UCatInterface::StaticClass());
	ICatInterface* Interface = Cast<ICatInterface>(OtherActor);

	if (!bIsImplemted) { return; }

	Interface->Execute_ReceiveDamage(OtherActor,dp);
	if (OtherActor->ActorHasTag("Item"))
	{
		GameMode->AddDashPoint(dp);
		OnAddDashPoint();
	}
	else if (OtherActor->ActorHasTag("Obstacle"))
	{
		if (GetMainGameMode()->IsDashing()) { return; }
		PlayAnimMontage(DamageAnimation);
		bIsDamaging = true;
		bIsHitObscle = true;
		Damage();
		
	}
	else if (OtherActor->ActorHasTag("Goal"))
	{
		//1秒経過後インプットをしないように
		GetWorldTimerManager().SetTimer(DisableInputTimerHandle,this, &ACat::AfterGoalEvent, 2.0f, false);
	}
}
void ACat::AfterGoalEvent()
{
	GameMode->RaceStop();
	DisableInput(PlayerController);
}


void ACat::Damage()
{
	DisableInput(PlayerController);

	FTimerHandle HitObscaleTimeHandle;
	GetWorldTimerManager().SetTimer(HitObscaleTimeHandle, this, &ACat::AfterHitObscale, GetMainGameMode()->RecordingDeltaTime, false);

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ACat::DamageFlashing, 0.1f, true);

	FTimerHandle DamageFinishTimerHandle;
	GetWorldTimerManager().SetTimer(DamageFinishTimerHandle, this, &ACat::OnFinishedDamage, MaxDamageTime, false);
	
}
void ACat::OnFinishedDamage()
{
	EnableInput(PlayerController);
	bIsDamaging = false;
	SetActorHiddenInGame(false);
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
}



void ACat::Dash()
{
	UE_LOG(LogTemp, Error, TEXT("DASHHHHH"));
	const int32 CurrentPoint = GetMainGameMode()->GetCurrentDashPoint();
	const int32 MaxPoint = GetMainGameMode()->GetMaxDashPoint();
	if (CurrentPoint >= MaxPoint)
	{
		if (bIsDamaging) return;
		if (GameMode->IsDashing()) return;
		GameMode->SetIsDashing(true);
		DecreasePointOnDashing();
		DashAction();
	}
}

void ACat::DashAction()
{
	SetMaxSpeedAndAccel(DashingMaxSpeed, DashingMaxAcceleration);
	FTimerHandle DashingTimerHandle;
	GetWorldTimerManager().SetTimer(DashingTimerHandle,this,&ACat::OnDashingEvent,2.0f,false);
}

AMainGameModeBase* ACat::GetMainGameMode()
{
	return Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
}

void ACat::DamageFlashing()
{
	bIsDamaging = true;
	SetActorHiddenInGame(bDamageFlashFlipFloop);
	bDamageFlashFlipFloop = !bDamageFlashFlipFloop;
}

void ACat::OnDashingEvent()
{
	SetMaxSpeedAndAccel(DefaultMaxSpeed, DefaultMaxAcceleration);
	GameMode->SetIsDashing(false);
	GameMode->SetCurrentDashPoint(0);
}

void ACat::AfterHitObscale()
{
	bIsHitObscle = false;
}

void ACat::SetMaxSpeedAndAccel(const float Speed, const float Accel)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MaxAcceleration = Accel;
}