// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"


UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidget(TEXT("/Game/FirstPersonCPP/Blueprints/WBP_GameOver"));

	if (!GameOverWidget.Class)
		return;

	GameOverWidgetClass = GameOverWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> StanceWgt(TEXT("/Game/FirstPersonCPP/Blueprints/WBP_Stance"));

	if (!StanceWgt.Class)
		return;

	//StanceWidgetClass = StanceWgt.Class;

	//StanceWidget = CreateWidget<UUserWidget>(this, StanceWidgetClass);
	//if (StanceWidget)
	//{
	//	StanceWidget->AddToViewport();
	//}
	
}

void UMyGameInstance::Init()
{
	
}

void UMyGameInstance::ShowGameOverWidget()
{
	//Create widget and add to viewport

	UUserWidget* GameOver = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	GameOver->AddToViewport();

	//Get ref to player controller
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	//Only UI responds to user input
	FInputModeUIOnly InputModeData;
	
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

}

