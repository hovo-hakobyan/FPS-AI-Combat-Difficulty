// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RuleManager.generated.h"


UCLASS()
class AI_ACCURACY_API URuleManager : public UObject
{
	GENERATED_BODY()

public:
	URuleManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* DistanceRuleTable;

private:


};
