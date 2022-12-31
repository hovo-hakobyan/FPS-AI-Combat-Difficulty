// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleManager.h"
#include "RuleTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"

URuleManager::URuleManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DistanceTable(TEXT("DataTable'/Game/DataTables/DistanceRuleTable.DistanceRuleTable'"));
	if (DistanceTable.Succeeded())
	{
		V_DistanceRuleTable = DistanceTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> StanceTable(TEXT("DataTable'/Game/DataTables/StanceRuleTable.StanceRuleTable'"));
	if (StanceTable.Succeeded())
	{
		S_StanceRuleTable = StanceTable.Object;
		InitStanceRuleTable();
	}
}

float URuleManager::GetMultiplier(const UDataTable& dataTable, float value)
{
	//Read all the data from the given table
	const FString context{ TEXT("Distance Rule Array Context") };
	TArray<FValueRule*> outRowArr;
	dataTable.GetAllRows(context, outRowArr);

	//Sort the array based on operator<
	outRowArr.Sort();

	//Smallest and biggest indexes
	int smallest{ 0 };
	int biggest{ outRowArr.Num() - 1 };

	//If our value is less than our smallest value, we return the smallest multiplier
	if (value <= outRowArr[smallest]->Value)
		return outRowArr[smallest]->Multiplier;
	
	
	//If our value is bigger than our biggest value, we return the biggest multiplier
	if (value >= outRowArr[biggest]->Value)
		return outRowArr[biggest]->Multiplier;
	

	float leftBorderIdx{};
	float rightBorderIdx{};

	for (int i = 0; i < outRowArr.Num(); ++i)
	{
		//If we have the exact value in our array, we stop and return the corresponding multiplier
		if (UKismetMathLibrary::EqualEqual_FloatFloat(outRowArr[i]->Value,value))
		{
			return outRowArr[i]->Multiplier;
		}


		//Get the left element index 
		if (outRowArr[i]->Value <= value)
		{
			leftBorderIdx = i;
		}

		//get the right element index
		if (outRowArr[outRowArr.Num() - 1 - i]->Value >= value)
		{
			rightBorderIdx = outRowArr.Num() - 1 - i;
		}
	}

	//If we made it so far, the value is between 2 other values
	//https://www.bbc.co.uk/bitesize/guides/z9387p3/revision/5#:~:text=The%20straight%20line%20through%20two,coordinates%20of%20the%20two%20points.
	// Formula for in between values y = mx + c
	float m = (outRowArr[rightBorderIdx]->Multiplier - outRowArr[leftBorderIdx]->Multiplier) / (outRowArr[rightBorderIdx]->Value - outRowArr[leftBorderIdx]->Value);
	float c = outRowArr[rightBorderIdx]->Multiplier - m * outRowArr[rightBorderIdx]->Value;

	return m * value + c;

}

void URuleManager::InitStanceRuleTable()
{
	TArray<FString> values;
	values.Add("Test");
	values.Add("Test2");

	for (int i = 0; i < values.Num(); i++)
	{
		FStringRule rule;
		rule.Description = "test" + i;
		rule.Value = values[i];
		rule.Multiplier = i;

		S_StanceRuleTable->AddRow(FName(TEXT("NewRow") + i), rule);
	}


}
