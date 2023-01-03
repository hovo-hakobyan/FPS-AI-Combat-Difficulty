// Fill out your copyright notice in the Description page of Project Settings.


#include "AITimerWidget.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Styling/SlateColor.h"
#include "Math/Color.h"

void UAITimerWidget::SetText(const FString& text)
{
	FText inText = FText::FromString(text);
	WidgetText->SetText(inText);
}

void UAITimerWidget::SetHitMissText(bool didHit)
{
	//Set text
	FString text = didHit ? "HIT" : "MISS";
	FText inText = FText::FromString(text);
	HitMissText->SetText(inText);

	//Set Color
	FSlateColor color = didHit ? FLinearColor::Green : FLinearColor::Red;
	HitMissText->SetColorAndOpacity(color);
}

void UAITimerWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
}
