// Fill out your copyright notice in the Description page of Project Settings.


#include "HP.h"

// Sets default values for this component's properties
UHP::UHP(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	max = 100;
	CurrentHealth = max;

	UE_LOG(LogTemp, Warning, TEXT("Here"));
}

void UHP::TakeDamage(int amount)
{

	CurrentHealth -= amount;

	if (CurrentHealth < 0)
	{
		CurrentHealth = 0;

	}
}

bool UHP::IsValidNum()
{
	return CurrentHealth != 0;
}

