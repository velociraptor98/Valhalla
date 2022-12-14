// Fill out your copyright notice in the Description page of Project Settings.


#include "ValhallaAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UValhallaAnimInstance::NativeInitializeAnimation()
{
    shooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UValhallaAnimInstance::updateAnimationProperties(float deltaTime)
{
    if(shooterCharacter == nullptr){
        shooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
    }
    if(shooterCharacter){
        FVector velocity{shooterCharacter->GetVelocity()};
        velocity.Z = 0;
        speed = velocity.Size();
        bIsInAir = shooterCharacter->GetCharacterMovement()->IsFalling();
        if(shooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size()>0){
            bIsAccelerating = true;
        }
        else{
            bIsAccelerating = false;
        }
        FRotator aimRotation = shooterCharacter->GetBaseAimRotation();
        FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(shooterCharacter->GetVelocity());
        movementOffset = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation,aimRotation).Yaw;
//        FString rotationMessage = FString::Printf(TEXT("Base aim rotation: %f"),movementOffset);
//        if(GEngine){
//            GEngine->AddOnScreenDebugMessage(1,0.0f,FColor::White,rotationMessage);
//        }
    }
}
