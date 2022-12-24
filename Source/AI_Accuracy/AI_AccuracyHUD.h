// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AI_AccuracyHUD.generated.h"

UCLASS()
class AAI_AccuracyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAI_AccuracyHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

