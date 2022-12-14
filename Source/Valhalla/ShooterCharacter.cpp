// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"



// Sets default values
AShooterCharacter::AShooterCharacter() : baseTurnRate(45.0f),baseLookupRate(45.0f),bAiming(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    cameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    cameraArm->SetupAttachment(RootComponent);
    cameraArm->TargetArmLength = 300.0f;
    //When camera moves use that rotation
    cameraArm->bUsePawnControlRotation = true;
    cameraArm->SocketOffset = FVector(0.0f,50.0f,50.0f);
    
    // Setup Camera
    followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
    followCamera->SetupAttachment(cameraArm, USpringArmComponent::SocketName);
    //Only change for spring arm, camera should not rotate relative to arm
    followCamera->bUsePawnControlRotation = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;
    //Configure Char Movement
    GetCharacterMovement()->bOrientRotationToMovement = false; // Char move in input direction
    GetCharacterMovement()->RotationRate = FRotator(0.0f,540.0f,0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;
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
    PlayerInputComponent->BindAxis("TurnRate",this,&AShooterCharacter::turnAtRate);
    PlayerInputComponent->BindAxis("LookupRate",this,&AShooterCharacter::lookupAtRate);
    PlayerInputComponent->BindAxis("Lookup",this,&APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);
    PlayerInputComponent->BindAction("FireButton",IE_Pressed,this,&AShooterCharacter::fireWeapon);
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

void AShooterCharacter::turnAtRate(float rate)
{
    AddControllerYawInput(rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::lookupAtRate(float rate)
{
    AddControllerPitchInput(rate * baseLookupRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::fireWeapon(){
    if(fireShot){
        UGameplayStatics::PlaySound2D(this,fireShot);
    }
    const USkeletalMeshSocket* barrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
    if(barrelSocket){
        const FTransform socketTransform = barrelSocket->GetSocketTransform(GetMesh());
        if(muzzleFlash){
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),muzzleFlash, socketTransform);
        }
        FVector beamEnd;
        bool bBeamEnd = getBeamEndLocation(socketTransform.GetLocation(),beamEnd);
        if(bBeamEnd){
            if(impactParticles){
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),impactParticles, beamEnd);
            }
            if(beamParticles){
                UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),beamParticles,socketTransform);
                if(beam){
                    beam->SetVectorParameter(FName("Target"),beamEnd);
                }
            }
        }
    UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
    if(animInstance && fireMontage){
        animInstance->Montage_Play(fireMontage);
        animInstance->Montage_JumpToSection(FName("StartFire"));
    }
}
}
bool AShooterCharacter::getBeamEndLocation(const FVector& muzzleSocketLocation,FVector& outBeamLocation){
    FVector2D viewportSize;
    if(GEngine && GEngine->GameViewport){
        GEngine->GameViewport->GetViewportSize(viewportSize);
    }
    FVector2D crosshairLocation(viewportSize.X/2.0f,viewportSize.Y/2.0f);
    crosshairLocation.Y-=50.0f;
    FVector crosshairWorldPosition;
    FVector crosshairWorldDirection;
    bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0),crosshairLocation,crosshairWorldPosition,crosshairWorldDirection);
    if(bScreenToWorld){
        FHitResult screenTraceHit;
        const FVector start{crosshairWorldPosition};
        const FVector end{crosshairWorldPosition + crosshairWorldDirection * 50000.0f};
        //FVector beamEndPoint{end};
        outBeamLocation = end;
        GetWorld()->LineTraceSingleByChannel(screenTraceHit,start,end,ECollisionChannel::ECC_Visibility);
        if(screenTraceHit.bBlockingHit){
            outBeamLocation = screenTraceHit.Location;
            FHitResult weaponTraceHit;
            const FVector weaponTraceStart{muzzleSocketLocation};
            const FVector weaponTraceEnd{outBeamLocation};
            GetWorld()->LineTraceSingleByChannel(weaponTraceHit,weaponTraceStart,weaponTraceEnd,ECollisionChannel::ECC_Visibility);
            if(weaponTraceHit.bBlockingHit){
                outBeamLocation = weaponTraceHit.Location;
            }
        }
        return true;
    }
    return false;
}

void AShooterCharacter::aimButtonPressed(){
    bAiming = true;
}
void AShooterCharacter::aimButtonReleased(){
    bAiming = false;
}
