// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blaster/CodeUtils/CodeUtils.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
	//<>
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);

	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);



}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (PickupWidget)
		PickupWidget->SetVisibility(false);


	if (HasAuthority()) {

		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);

		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
		Ammo = MagCapacity;
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME(AWeapon, Ammo);
}

void AWeapon::OnRep_Owner()
{
	{
		Super::OnRep_Owner();

		if (Owner == nullptr)
		{
			BlasterOwnerCharacter = nullptr;
			BlasterOwnerController = nullptr;
		}
		else
		{
			BlasterOwnerCharacter = Cast<ABlasterCharacter>(GetOwner());
			if (BlasterOwnerCharacter)
			{
				BlasterOwnerController = Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller);
			}

			SetHUDWeaponAmmoVisible(true);
			SetHUDWeaponAmmo();
		}
	}
}

/////////////

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SeweepResult)
{
	ABlasterCharacter* blasterCharater = Cast<ABlasterCharacter>(OtherActor);
	if (blasterCharater)
	{
		blasterCharater->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABlasterCharacter* blasterCharater = Cast<ABlasterCharacter>(OtherActor);
	if (blasterCharater)
	{
		blasterCharater->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHUDWeaponAmmo();
}

void AWeapon::OnRep_Ammo()
{
	SetHUDWeaponAmmo();
}

void AWeapon::SetHUDWeaponAmmo()
{
	ABlasterCharacter* BlasterChar = Cast<ABlasterCharacter>(GetOwner());
	if (BlasterChar)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(BlasterChar->Controller);
		if (BlasterPlayerController && BlasterPlayerController->IsLocalController())
		{
			BlasterPlayerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

void AWeapon::SetHUDWeaponAmmoVisible(bool Visibility)
{
	ABlasterCharacter* BlasterChar = Cast<ABlasterCharacter>(GetOwner());
	if (BlasterChar)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(BlasterChar->Controller);
		if (BlasterPlayerController && BlasterPlayerController->IsLocalController())
		{
			BlasterPlayerController->SetHUDWeaponAmmoVisible(Visibility);
			return;
		}
	}


	if (!Visibility && GetWorld())
	{
		ABlasterPlayerController* LocalPlayerController = Cast<ABlasterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (LocalPlayerController && LocalPlayerController->IsLocalController())
		{
			if (LocalPlayerController->GetPawn() == BlasterOwnerCharacter)
			{
				LocalPlayerController->SetHUDWeaponAmmoVisible(false);
			}
		}
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EWeaponState::EWS_Dropped:
		SetHUDWeaponAmmoVisible(false);
		if (HasAuthority())
		{
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EWeaponState::EWS_Dropped:
		SetHUDWeaponAmmoVisible(false);
		if (HasAuthority())
		{
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
	}
	SpendRound();
}

void AWeapon::Drop()
{
	SetWeaponState(EWeaponState::EWS_Dropped);

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);

	if (GetWorld())
	{
		FTimerHandle DetachOwnerTimer;
		GetWorld()->GetTimerManager().SetTimer(
			DetachOwnerTimer,
			this,
			&AWeapon::ClearWeaponOwner,
			0.05f,
			false
		);
	}
}

void AWeapon::ClearWeaponOwner()
{
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}

