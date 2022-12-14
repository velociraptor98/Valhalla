// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class VALHALLA_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void moveForward(float val);
    void moveRight(float val);
    void turnAtRate(float rate);
    void lookupAtRate(float rate);
    void fireWeapon();
    bool getBeamEndLocation(const FVector& muzzleSocketLocation,FVector& outBeamLocation);
    void aimButtonPressed();
    void aimButtonReleased();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* cameraArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* followCamera;
    //In Degrees per second
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float baseTurnRate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float baseLookupRate;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapons",meta = (AllowPrivateAccess = "true"))
    class USoundCue* fireShot;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapons",meta = (AllowPrivateAccess = "true"))
    class UParticleSystem* muzzleFlash;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapons",meta = (AllowPrivateAccess = "true"))
    class UAnimMontage* fireMontage;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapons",meta = (AllowPrivateAccess = "true"))
    UParticleSystem* impactParticles;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapons",meta = (AllowPrivateAccess = "true"))
    UParticleSystem* beamParticles;
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Weapons",meta = (AllowPrivateAccess = "true"))
    bool bAiming;

    
    
    
public:
    FORCEINLINE USpringArmComponent* getCameraArm() const { return this->cameraArm; }
    FORCEINLINE UCameraComponent* getFollowCamera() const { return this->followCamera; }
};
