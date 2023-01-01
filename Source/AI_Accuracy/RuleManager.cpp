// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleManager.h"
#include "RuleTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI_AccuracyCharacter.h"

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
		ValidateStringTableStance();
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DirTable(TEXT("DataTable'/Game/DataTables/PlayerDirTable.PlayerDirTable'"));
	if (DirTable.Succeeded())
	{
		V_DirectionRuleTable = DirTable.Object;
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

float URuleManager::GetMultiplierFromString(const UDataTable& dataTable, const FString& value)
{
	//Get the row based on the RowName
	const FString context{ TEXT("Find multiplier from string") };
	FStringRule* row = dataTable.FindRow<FStringRule>(FName(*value), context);

	//If the row exists
	if (row)
	{
		//If the content of the row is valid
		if (row->isValid)
		{
			return row->Multiplier;
		}
	}

	//If invalid row then we want this multiplayer to have no effect;
	return 1.f;
}

void URuleManager::ValidateStringTableStance()
{
	//Read all the data from the given table
	const FString context{ TEXT("Stance rule context")};
	TArray<FStringRule*> outRowArr;
	S_StanceRuleTable->GetAllRows(context, outRowArr);

	for (int i = 0; i < outRowArr.Num(); i++)
	{
		for (ECharacterStance stance : TEnumRange<ECharacterStance>())
		{
			if (outRowArr[i]->Value == UEnum::GetDisplayValueAsText(stance).ToString())
			{
				outRowArr[i]->isValid = true;
				break;
				
			}
		}
	}

	

}
