// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AITimerWidget.generated.h"

class UTextBlock;

UCLASS()
class AI_ACCURACY_API UAITimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetText(const FString& text);
	void SetHitMissText(bool didHit);

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WidgetText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HitMissText;
};
