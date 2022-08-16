// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    cameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    cameraArm->SetupAttachment(RootComponent);
    cameraArm->TargetArmLength = 300.0f;
    //When camera moves use that rotation
    cameraArm->bUsePawnControlRotation = true;
    
    // Setup Camera
    followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
    followCamera->SetupAttachment(cameraArm, USpringArmComponent::SocketName);
    //Only change for spring arm, camera should not rotate relative to arm
    followCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp,Warning,TEXT("INIT"));
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward",this,&AShooterCharacter::moveForward);
    PlayerInputComponent->BindAxis("MoveRight",this,&AShooterCharacter::moveRight);
}
// Back and forward movement
void AShooterCharacter::moveForward(float val)
{
    if((Controller!=nullptr) && val != 0.0f)
    {
        const FRotator Rotation{Controller->GetControlRotation()};
        const FRotator YawRotation{0,Rotation.Yaw,0};
        const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
        AddMovementInput(Direction,val);
    }
}
// Right and left movement
void AShooterCharacter::moveRight(float val)
{
    if((Controller!=nullptr) && val != 0.0f)
    {
        const FRotator Rotation{Controller->GetControlRotation()};
        const FRotator YawRotation{0,Rotation.Yaw,0};
        const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};
        AddMovementInput(Direction,val);
    }
}

