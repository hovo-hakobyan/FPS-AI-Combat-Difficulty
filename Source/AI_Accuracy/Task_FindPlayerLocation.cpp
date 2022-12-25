// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Blackboard_Keys.h"


UTask_FindPlayerLocation::UTask_FindPlayerLocation(const FObjectInitializer& objectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* pNodeMemory)
{
	//Get player character
	ACharacter* const pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);

	

	if (!pPlayer)
		return EBTNodeResult::Failed;

	//Get AI controller
	const auto controller = Cast<AMyAIController>(ownerComponent.GetAIOwner());

	if (!controller)
		return EBTNodeResult::Failed;

	//Change data in BB
	controller->GetBB()->SetValueAsObject(bb_keys::target, pPlayer);

	//Finish task
	FinishLatentTask(ownerComponent, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
