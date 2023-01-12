// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RuleTypes.h"
#include "RuleManager.generated.h"

class UDataTable;

UENUM()
enum class EValueRule : uint8
{
	DistanceRule,
	VelocityRule,
};

UENUM()
enum class EStringRule : uint8
{
	//Existing string rules
	StanceRule,
};


UCLASS()
class AI_ACCURACY_API URuleManager : public UObject
{
	GENERATED_BODY()

public:
	URuleManager();

	/**Pointer to DistanceTable created in the engine*/
	UPROPERTY()
	UDataTable* V_DistanceRuleTable;

	/**Pointer to StanceTable created in the engine*/
	UPROPERTY()
	UDataTable* S_StanceRuleTable;

	/**Pointer to VelocityTable created in the engine*/
	UPROPERTY()
	UDataTable* V_VelocityRuleTable;
	
	float GetMultiplier(const EValueRule& valueRule, float value);
	float GetMultiplierFromString(const EStringRule& valueRule, const FString& value);

	UDataTable* GetStanceRuleTable() { return S_StanceRuleTable; }
	UDataTable* GetDistanceRuleTable() { return V_DistanceRuleTable; }
	UDataTable* GetVelocityRuleTable() { return V_VelocityRuleTable; }
private:

	void ValidateStringTableStance();

	/**Arrays to cache table content for later use*/
	TArray<FValueRule*> DistanceRuleArr;
	TArray<FValueRule*> VelocityRuleArr;
	TArray<FStringRule*> StanceRuleArr;
};
