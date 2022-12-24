// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_AIShoot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blackboard_Keys.h"




UTask_AIShoot::UTask_AIShoot(const FObjectInitializer& objectInitializer)
{
	//Add display name to the node

	NodeName = TEXT("Shoot at target");
}

EBTNodeResult::Type UTask_AIShoot::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* pNodeMemory)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("sdfafs"));
	}


	//Finish task with success
	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
