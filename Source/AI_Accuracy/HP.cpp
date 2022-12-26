// Fill out your copyright notice in the Description page of Project Settings.


#include "HP.h"

// Sets default values for this component's properties
UHP::UHP(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	max = 100;
	current = max;

}

void UHP::TakeDamage(int amount)
{

	current -= amount;

	if (current < 0)
	{
		current = 0;
	}
}

bool UHP::IsValid()
{
	return current != 0;
}


