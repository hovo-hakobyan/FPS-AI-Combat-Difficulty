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
	PrimaryActorTick.bCanEverTick = true;
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
	

	check(RuleManager);
	
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();


	//Fire up the behavior tree
	RunBehaviorTree(pBehaviorTree);
	pBehaviorTreeComponent->StartTree(*pBehaviorTree);

	//Get player ref
	PlayerCharRef = Cast<AAI_AccuracyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(PlayerCharRef);

	//Get controlled pawn ref
	ControlledPawnRef = Cast<AEnemy>(GetPawn());
	check(ControlledPawnRef);
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsAlert)
	{
		//Calculate values to get multipliers
		float distanceValue = FVector::Distance(ControlledPawnRef->GetActorLocation(), PlayerCharRef->GetActorLocation());
		float dirAngleDiff = 0;

		//Convert distance to meters
		distanceValue = FUnitConversion::Convert(distanceValue, EUnit::Centimeters, EUnit::Meters);

		
		//Calculate multipliers
		float distanceMultiplier = RuleManager->GetMultiplier(*RuleManager->GetDistanceRuleTable(), distanceValue);
		float stanceMultiplier = RuleManager->GetMultiplierFromString(*RuleManager->GetStanceRuleTable(), UEnum::GetDisplayValueAsText(PlayerCharRef->GetCharacterStance()).ToString());

		//Calculate final delay
		float finalDelay = baseDelay * distanceMultiplier * stanceMultiplier;

		//Pass it to controlled pawn
		ControlledPawnRef->SetFinalDelay(finalDelay);
		
	}
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


	//Update BB value CanSeePlayer
	pBlackboard->SetValueAsBool(bb_keys::canSeePlayer, stimulus.WasSuccessfullySensed());
	SetFocus(stimulus.WasSuccessfullySensed() ? PlayerCharRef : nullptr);
	

	// Determins whether AI can shoot at the player (aka can AI see the player)
	IsAlert = !IsAlert;
	ControlledPawnRef->SetCanShoot(IsAlert);

}

void AMyAIController::SetupPerceptionSystem()
{
	//create and initialize sight config object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	//Player withing this radius == detected
	SightConfig->SightRadius = 5000.0f;

	//Radius to leave sight
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;

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




