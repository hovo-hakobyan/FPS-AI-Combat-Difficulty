// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StanceWidget.generated.h"

class UTexture2D;
class UImage;

UCLASS()
class AI_ACCURACY_API UStanceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetImage(UTexture2D* InTexture);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* StanceImage;
};
