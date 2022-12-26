// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_AIShoot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blackboard_Keys.h"
#include "Enemy.h"
#include "AI_AccuracyCharacter.h"



UTask_AIShoot::UTask_AIShoot(const FObjectInitializer& objectInitializer)
{
	//Add display name to the node

	NodeName = TEXT("Shoot at target");
}

EBTNodeResult::Type UTask_AIShoot::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* pNodeMemory)
{
	AMyAIController* pAIController = Cast<AMyAIController>(ownerComp.GetAIOwner());


	if (!pAIController)
		return EBTNodeResult::Failed;

	pCharacter = Cast<AEnemy>(pAIController->GetPawn());

	if (!pCharacter)
		return EBTNodeResult::Failed;

	AAI_AccuracyCharacter* pTarget = Cast<AAI_AccuracyCharacter>(pAIController->GetBB()->GetValueAsObject(bb_keys::target));

	if (!pTarget)
		return EBTNodeResult::Failed;

	AEnemy* pShooter = Cast<AEnemy>(pAIController->GetPawn());

	
	if (!pShooter)
		return EBTNodeResult::Failed;

	pShooter->Shoot(true);

	//Finish task with success
	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
