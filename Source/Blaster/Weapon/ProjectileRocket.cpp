// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* FiringPawn = GetInstigator();
	if (FiringPawn)
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			// Play impact sound for the firing player
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,
				Damage, // Base damage
				10.f, // Minimum damage
				GetActorLocation(),
				100.f, // Inner radius
				200.f, // Outer radius
				1.f, // Damage falloff
				UDamageType::StaticClass(),
				TArray<AActor*>(), // Ignore actors
				this,
				FiringController,
				ECC_Visibility
			);
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
