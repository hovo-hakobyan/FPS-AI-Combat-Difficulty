// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RuleTypes.generated.h"


USTRUCT(BlueprintType)
struct AI_ACCURACY_API FStringRule : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multiplier;

	UPROPERTY(BlueprintReadOnly)
	FString Value;

	UPROPERTY(BlueprintReadOnly)
	FString Description;
};

USTRUCT(BlueprintType)
struct AI_ACCURACY_API FValueRule : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;

	FORCEINLINE bool operator<(const FValueRule& other) const
	{
		return Multiplier < other.Multiplier;
	}
};