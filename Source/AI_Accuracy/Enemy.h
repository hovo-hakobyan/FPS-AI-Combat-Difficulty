// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Enemy.generated.h"

class UWidgetComponent;

UCLASS()
class AI_ACCURACY_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();


	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* GunMesh;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* ParticleSpawn;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	class UParticleSystemComponent* PS_GunFireComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* PS_GunFire;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMyProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	class UWidgetComponent* TimerWidgetComponent;
	
	UPROPERTY(EditAnywhere)
	class UAITimerWidget* TimerWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Shoot();

	void SetFinalDelay(float delay) { finalDelay = delay; }
	void SetCanShoot(bool shoot) { canShoot = shoot; }
	bool GetCanShoot()const { return canShoot; }

private:
	float finalDelay;
	float currentDelay;
	bool canShoot;
	bool shouldHit;

};
