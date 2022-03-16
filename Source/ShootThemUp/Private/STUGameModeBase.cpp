// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPlayerController.h"
#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase() {
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
}