// Fill out your copyright notice in the Description page of Project Settings.


#include "StanceWidget.h"
#include "Components/Image.h"

void UStanceWidget::SetImage(UTexture2D* InTexture)
{
	StanceImage->SetBrushFromTexture(InTexture);
}
