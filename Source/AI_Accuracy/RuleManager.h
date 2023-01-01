// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RuleManager.generated.h"

class UDataTable;

UCLASS()
class AI_ACCURACY_API URuleManager : public UObject
{
	GENERATED_BODY()

public:
	URuleManager();

	UPROPERTY()
	UDataTable* V_DistanceRuleTable;
	UPROPERTY()
	UDataTable* S_StanceRuleTable;
	UPROPERTY()
	UDataTable* V_DirectionRuleTable;
	

	float GetMultiplier(const UDataTable& dataTable, float value);
	float GetMultiplierFromString(const UDataTable& dataTable, const FString& value);

	UDataTable* GetDistanceRuleTable() { return V_DistanceRuleTable; }
	UDataTable* GetStanceRuleTable() { return S_StanceRuleTable; }
	UDataTable* GetDirectionRuleTable() { return V_DirectionRuleTable; }

private:

	void ValidateStringTableStance();

};
