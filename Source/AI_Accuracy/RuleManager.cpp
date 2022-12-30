// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleManager.h"
#include "RuleTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

URuleManager::URuleManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DistanceTable(TEXT("DataTable'/Game/DataTables/DistanceRuleTable.DistanceRuleTable'"));
	if (DistanceTable.Succeeded())
	{
		DistanceRuleTable = DistanceTable.Object;
	}

	if (DistanceRuleTable)
	{
		const FString context{ TEXT("Distance Context") };
		FValueRule* firstMultiplier = DistanceRuleTable->FindRow<FValueRule>(FName(TEXT("First")), context, true);

	}
}
