// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleManager.h"
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

		//Cache the table in array and pre sort
		const FString context{ TEXT("Distance Rule Array Context") };
		V_DistanceRuleTable->GetAllRows(context, DistanceRuleArr);
		DistanceRuleArr.Sort();
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> StanceTable(TEXT("DataTable'/Game/DataTables/StanceRuleTable.StanceRuleTable'"));
	if (StanceTable.Succeeded())
	{
		S_StanceRuleTable = StanceTable.Object;
		ValidateStringTableStance();
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> VelTable(TEXT("DataTable'/Game/DataTables/VelocityRuleTable.VelocityRuleTable'"));
	if (VelTable.Succeeded())
	{
		V_VelocityRuleTable = VelTable.Object;

		//Cache the table in array and pre sort
		const FString context{ TEXT("Velocity Rule Array Context") };
		V_VelocityRuleTable->GetAllRows(context, VelocityRuleArr);
		VelocityRuleArr.Sort();
	}
}

float URuleManager::GetMultiplier(const EValueRule& valueRule, float value)
{
	//avoids copying the array
	TArray<FValueRule*>* outRowArrPtr = nullptr;

	switch (valueRule)
	{
	case EValueRule::DistanceRule:
		outRowArrPtr = &DistanceRuleArr;
		break;
	case EValueRule::VelocityRule:
		outRowArrPtr = &VelocityRuleArr;
		break;
	}
		
	if (!outRowArrPtr)
		return 1.f;

	//Smallest and biggest indexes (array sorted)
	int smallest{ 0 };
	int biggest{ outRowArrPtr->Num() - 1 };

	//If our value is less than our smallest value, we return the smallest multiplier
	if (value <= (*outRowArrPtr)[smallest]->Value)
		return (*outRowArrPtr)[smallest]->Multiplier;
	
	
	//If our value is bigger than our biggest value, we return the biggest multiplier
	if (value >= (*outRowArrPtr)[biggest]->Value)
		return (*outRowArrPtr)[biggest]->Multiplier;
	

	float leftBorderIdx{};
	float rightBorderIdx{};

	//Go over every element of table (cached array)
	for (int i = 0; i < outRowArrPtr->Num(); ++i)
	{
		//If we have the exact value in our array, we stop and return the corresponding multiplier
		if (UKismetMathLibrary::EqualEqual_FloatFloat((*outRowArrPtr)[i]->Value,value))
		{
			return (*outRowArrPtr)[i]->Multiplier;
		}


		//Get the left element index 
		if ((*outRowArrPtr)[i]->Value <= value)
		{
			leftBorderIdx = i;
		}

		//get the right element index
		if ((*outRowArrPtr)[outRowArrPtr->Num() - 1 - i]->Value >= value)
		{
			rightBorderIdx = outRowArrPtr->Num() - 1 - i;
		}
	}

	//If we made it so far, the value is between 2 other values
	//https://www.bbc.co.uk/bitesize/guides/z9387p3/revision/5#:~:text=The%20straight%20line%20through%20two,coordinates%20of%20the%20two%20points.
	// Formula for in between values y = mx + c
	float m = ((*outRowArrPtr)[rightBorderIdx]->Multiplier - (*outRowArrPtr)[leftBorderIdx]->Multiplier) / ((*outRowArrPtr)[rightBorderIdx]->Value - (*outRowArrPtr)[leftBorderIdx]->Value);
	float c = (*outRowArrPtr)[rightBorderIdx]->Multiplier - m * (*outRowArrPtr)[rightBorderIdx]->Value;

	return m * value + c;

}

float URuleManager::GetMultiplierFromString(const EStringRule& valueRule, const FString& value)
{
	//Get the row based on the RowName
	const FString context{ TEXT("Find multiplier from string") };
	FStringRule* row = nullptr;

	switch (valueRule)
	{
	case EStringRule::StanceRule:
		row = S_StanceRuleTable->FindRow<FStringRule>(FName(*value), context);
		break;
	}

	//If the row exists
	if (row)
	{
		//If the content of the row is valid
		if (row->isValid)
			return row->Multiplier;
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

	//Validate every row inside the data table
	for (int i = 0; i < outRowArr.Num(); i++)
	{
		outRowArr[i]->Description = "INVALID VALUE";
		outRowArr[i]->isValid = false;

		//Custom enum class that keeps information about player stances
		for (ECharacterStance stance : TEnumRange<ECharacterStance>())
		{
			if (outRowArr[i]->Value == UEnum::GetDisplayValueAsText(stance).ToString())
			{
				outRowArr[i]->isValid = true;
				outRowArr[i]->Description = "VALID VALUE";
				break;
			}
		}
	}

	//Copy the validated data to our local array
	StanceRuleArr = outRowArr;

}
