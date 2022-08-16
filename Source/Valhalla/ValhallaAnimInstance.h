// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ValhallaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VALHALLA_API UValhallaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    virtual void NativeInitializeAnimation() override;
    UFUNCTION(BlueprintCallable)
    void updateAnimationProperties(float deltaTime);
private:
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement, meta = (AllowPrivateAccess = "true"))
    class AShooterCharacter* shooterCharacter;
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement, meta = (AllowPrivateAccess = "true"))
    float speed;
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement, meta = (AllowPrivateAccess = "true"))
    bool bIsInAir;
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement, meta = (AllowPrivateAccess = "true"))
    bool bIsAccelerating;
};
