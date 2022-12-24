// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Task_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AI_ACCURACY_API UTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UTask_FindPlayerLocation(const FObjectInitializer& objectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* pNodeMemory);
protected:


private:
};
