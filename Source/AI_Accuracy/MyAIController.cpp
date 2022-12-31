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
#include "RuleManager.h"
#include "AI_AccuracyCharacter.h"
#include "Math/Vector.h"
#include "Math/UnitConversion.h"


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

	SetupPerceptionSystem();
	
	RuleManager = CreateDefaultSubobject<URuleManager>(TEXT("AI Rule Manager"));
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


void AMyAIController::OnPerception(AActor* actor,const FAIStimulus stimulus)
{
	const auto character = Cast<AAI_AccuracyCharacter>(actor);

	if (!character)
		return;

	const auto controlledPawn = Cast<AEnemy>(GetPawn());
	if (!controlledPawn)
		return;

	//Update BB value CanSeePlayer
	pBlackboard->SetValueAsBool(bb_keys::canSeePlayer, stimulus.WasSuccessfullySensed());
	SetFocus(stimulus.WasSuccessfullySensed() ? character : nullptr);
	
	if (!RuleManager)
		return;

	//Calculate values to get multipliers
	float distanceValue = FVector::Distance(character->GetActorLocation(), controlledPawn->GetActorLocation());

	//Convert distance to meters
	distanceValue = FUnitConversion::Convert(distanceValue, EUnit::Centimeters, EUnit::Meters);

	//Calculate multipliers
	float distanceMultiplier = RuleManager->GetMultiplier(*RuleManager->GetDistanceRuleTable(), distanceValue);
	
	//Calculate final delay
	float finalDelay = baseDelay * distanceMultiplier;

	//Pass it to controlled pawn
	controlledPawn->SetFinalDelay(finalDelay);

	// Determins whether AI can shoot at the player (aka can AI see the player)
	controlledPawn->SetCanShoot(!controlledPawn->GetCanShoot());


}

void AMyAIController::SetupPerceptionSystem()
{
	//create and initialize sight config object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	//Player withing this radius == detected
	SightConfig->SightRadius = 2000.0f;

	//Radius to leave sight
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;

	//FOV
	SightConfig->PeripheralVisionAngleDegrees = 90.f;

	//Time (in seconds) to forget last seen loc
	SightConfig->SetMaxAge(5.f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	//add sight config component to perception component
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());


	//When something is perceieved, call the function
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerception);
}




