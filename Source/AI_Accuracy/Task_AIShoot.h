// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_AIShoot.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AI_ACCURACY_API UTask_AIShoot : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UTask_AIShoot(const FObjectInitializer& objectInitializer);

	//Task execution logic + return succeeded, failed, abort or inprogress
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* pNodeMemory);

protected:

private:
	class AEnemy* pCharacter = nullptr;

};
