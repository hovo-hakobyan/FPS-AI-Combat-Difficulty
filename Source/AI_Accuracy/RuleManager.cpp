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

}

float URuleManager::GetMultiplier(const UDataTable& dataTable, float value)
{
	const FString context{ TEXT("Distance Rule Array Context") };
	TArray<FValueRule*> outRowArr;
	DistanceRuleTable->GetAllRows(context, outRowArr);

	//To be used in multiplier calculation
	float lowMultiplier{INFINITY};
	float lowValue{ INFINITY };
	float highValue{ -INFINITY };


	for (int i = 0; i < outRowArr.Num(); ++i)
	{
		//Finds the lowest multiplier in the Data Table
		if (outRowArr[i]->Multiplier < lowMultiplier)
		{
			lowMultiplier = outRowArr[i]->Multiplier;
		}

		if (outRowArr[i]->Value < lowValue)
		{
			lowValue = outRowArr[i]->Value;
		}

		if (outRowArr[i]->Value > highValue)
		{
			highValue = outRowArr[i]->Value;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Lowest value is: %f"), lowValue);
	//UE_LOG(LogTemp, Warning, TEXT("Highest value is: %f"), highValue);

	float multiplier{ lowMultiplier + (lowMultiplier / (highValue - lowValue)) * value };
	UE_LOG(LogTemp, Warning, TEXT("Result value is: %f"), multiplier);
	return multiplier;
}
