// Fill out your copyright notice in the Description page of Project Settings.


#include "STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

void ASTURifleWeapon::StartFire() {
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire() {
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}
void ASTURifleWeapon::MakeShot() {
	if (!GetWorld()) return;
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;
	FHitResult HitResult;
	MakeHit(TraceStart, TraceEnd, HitResult);
	if (HitResult.bBlockingHit) {
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

	}
	else {
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) {
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)/*!GetPlayerViewPoint(ViewLocation, ViewRotation)*/) return false;
	
	bool a =GetPlayerViewPoint(ViewLocation, ViewRotation);

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}
