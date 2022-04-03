// Fill out your copyright notice in the Description page of Project Settings.


#include "STULauncherWeapon.h"
#include "STUProjectile.h"

void ASTULauncherWeapon::StartFire() {
	MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
	if (!GetWorld()) return;
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;
	FHitResult HitResult;
	MakeHit(TraceStart, TraceEnd, HitResult);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();


	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile) {
		Projectile->SetShotDirection(Direction);
		Projectile->FinishSpawning(SpawnTransform);
	}
}