// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD() {
	Super::DrawHUD();

	DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair() {
	
	const TInterval < float > Center(Canvas->SizeX*0.5f, Canvas->SizeY*0.5f);

	const float HalfLine = 5.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;
	DrawLine(Center.Min - HalfLine, Center.Max, Center.Min+HalfLine, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLine, Center.Min, Center.Max + HalfLine, LineColor, LineThickness);
}