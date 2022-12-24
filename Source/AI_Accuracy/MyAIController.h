// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AI_ACCURACY_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	//When controller possesses a pawn
	virtual void OnPossess(APawn* const pawn) override;

	class UBlackboardComponent* GetBB() const;

	
protected:

	//Used to implement senses for an actor
	UPROPERTY(VisibleAnywhere)
		UAIPerceptionComponent* pAIPerceptionComponent;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* pBehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* pBehaviorTree;

	class UBlackboardComponent* pBlackboard;

	//Sight sense for AI
	class UAISenseConfig_Sight* pSightConfig;


	UFUNCTION()
	void OnPerception(AActor* actor,const FAIStimulus stimulus);

	void SetupPerceptionSystem();
};
