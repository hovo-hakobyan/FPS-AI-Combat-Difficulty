// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AI_ACCURACY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UMyGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void ShowGameOverWidget();

protected:
	/*UPROPERTY()
	class UUserWidget* StanceWidget;*/

private:
	TSubclassOf<class UUserWidget> GameOverWidgetClass;

	//TSubclassOf<class UUserWidget> StanceWidgetClass;

	
};
