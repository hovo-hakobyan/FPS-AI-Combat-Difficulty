// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "AI_AccuracyCharacter.h"
#include "HP.h"


// Sets default values
AMyProjectile::AMyProjectile()
{
	//Create the component
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));

	CollisionComp->InitSphereRadius(5.f);
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	//Call OnHit every time OnComponentHit gets fired
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);

	//Player can't step on this 
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride{ WalkableSlope_Unwalkable,0.f });
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	//Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 6000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 0.0f;

}

void AMyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	AAI_AccuracyCharacter* pChar = Cast<AAI_AccuracyCharacter>(OtherActor);

	if (pChar)
	{
		if (pChar->Health)
		{
			int damageAmount{ 10};
			pChar->Health->TakeDamage(damageAmount);
			pChar->PlayBloodAnimation();
		}
	}
		
	Destroy();
}
