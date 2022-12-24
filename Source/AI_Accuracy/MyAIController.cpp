// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Enemy.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Blackboard_Keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "AI_AccuracyCharacter.h"


AMyAIController::AMyAIController(const FObjectInitializer& objectInitializer)
{
	//Look for behavior tree in path
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/FPEnemy/Blueprint/MyEnemy_BT.MyEnemy_BT'"));

	//If found set it to our BT
	if (obj.Succeeded())
	{
		pBehaviorTree = obj.Object;
	}

	pBehaviorTreeComponent = objectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComponent"));
	pBlackboard = objectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));

	SetupPerceptionSysem();
	
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();


	//Fire up the behavior tree
	RunBehaviorTree(pBehaviorTree);
	pBehaviorTreeComponent->StartTree(*pBehaviorTree);

}

void AMyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	if (pBlackboard)
	{
		//Sets which BB the BT should use
		pBlackboard->InitializeBlackboard(*pBehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AMyAIController::GetBB() const
{
	return pBlackboard;
}

void AMyAIController::OnTargetDetected(AActor* actor,const FAIStimulus stimulus)
{
	//actor must have UAIPerceptionSimuliSourceComponent in order to be detected
	if (const auto character = Cast<AAI_AccuracyCharacter>(actor) )
	{
		pBlackboard->SetValueAsBool(bb_keys::canSeePlayer, stimulus.WasSuccessfullySensed());
	}

}

void AMyAIController::SetupPerceptionSysem()
{
	//create and initialize sight config object
	pSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception config")));

	//Player withing this radius == detected
	pSightConfig->SightRadius = 500.0f;

	//Radius to leave sight
	pSightConfig->LoseSightRadius = pSightConfig->SightRadius + 50.f;

	//FOV
	pSightConfig->PeripheralVisionAngleDegrees = 90.f;

	//Time (in seconds) to forget last seen loc
	pSightConfig->SetMaxAge(5.f);

	pSightConfig->AutoSuccessRangeFromLastSeenLocation = 900.f;
	pSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	pSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	pSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//add sight config component to perception component
	GetPerceptionComponent()->SetDominantSense(*pSightConfig->GetSenseImplementation());

	//When something is perceieved, call the function
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnTargetDetected);

	GetPerceptionComponent()->ConfigureSense(*pSightConfig);

}




