// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HP.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_ACCURACY_API UHP : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHP(const FObjectInitializer& ObjectInitializer);

	int Get() const { return current; } 

	void TakeDamage(int amount);
	bool IsValid();

private:
	int current;
	int max;
};
