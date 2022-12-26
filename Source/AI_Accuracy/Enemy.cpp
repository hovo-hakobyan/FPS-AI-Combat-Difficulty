// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "MyProjectile.h"
#include "AI_AccuracyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyGun"));
	GunMesh->SetOnlyOwnerSee(false);			
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	GunMesh->SetupAttachment(GetMesh(), TEXT("GunSocket"));
	GunMesh->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	ParticleSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ParticleSpawnLocation"));
	ParticleSpawn->SetupAttachment(GunMesh);


	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	PS_GunFireComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponParticle"));
	PS_GunFireComp->SetupAttachment(GunMesh);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GunSocket"));

	GunMesh->SetHiddenInGame(false, false);
	GetMesh()->SetHiddenInGame(false, false);
	
}

void AEnemy::Shoot(bool shouldHit)
{
	
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator randomRotation = shouldHit ? FRotator{ 0.f,0.f,0.f } : FRotator{ 0.f,10.f,0.f };
			const FRotator SpawnRotation = GetControlRotation() + randomRotation;
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			
		}
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	//Play firing montage
	if (FireAnimation)
	{

		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (AnimInstance->Montage_Play(FireAnimation, 1.f) == 0)
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed to play"));
			}
		}
	}

	//Spawn particle
	if (PS_GunFireComp)
	{
		PS_GunFireComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_GunFire, ParticleSpawn->GetComponentLocation(),FRotator::ZeroRotator,FVector(0.005f));
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

