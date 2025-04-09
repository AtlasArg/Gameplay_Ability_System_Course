// Copyright Dry Horns 


#include "LKBeamSpell.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Aura/AbilitySystem/LKAbilitySystemLibrary.h"

void ULKBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        MouseHitLocation = HitResult.ImpactPoint;
        MouseHitActor = HitResult.GetActor();
    }
    else
    {
        CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
    }
}

void ULKBeamSpell::StoreOwnerVariables()
{
    if (CurrentActorInfo)
    {
        OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
        OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
    }
}

void ULKBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
    check(OwnerCharacter);
    if (OwnerCharacter->Implements<ULKCombatInterface>())
    {
        if (USkeletalMeshComponent* Weapon = ILKCombatInterface::Execute_GetWeapon(OwnerCharacter))
        {
            TArray<AActor*> ActorsToIgnore;
            ActorsToIgnore.Add(OwnerCharacter);
            FHitResult HitResult;
            const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
            UKismetSystemLibrary::SphereTraceSingle(
                OwnerCharacter,
                SocketLocation,
                BeamTargetLocation,
                10.f,
                TraceTypeQuery1,
                false,
                ActorsToIgnore,
                EDrawDebugTrace::None,
                HitResult,
                true);

            if (HitResult.bBlockingHit)
            {
                MouseHitLocation = HitResult.ImpactPoint;
                MouseHitActor = HitResult.GetActor();
            }
        }
    }

    if (ILKCombatInterface* CombatInterface = Cast<ILKCombatInterface>(MouseHitActor))
    {
        if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied))
        {
            CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::PrimaryTargetDied);
        }
    }
}

void ULKBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
    ActorsToIgnore.Add(MouseHitActor);

    TArray<AActor*> OverlappingActors;
    ULKAbilitySystemLibrary::GetLivePlayersWithinRadius(
        GetAvatarActorFromActorInfo(),
        OverlappingActors,
        ActorsToIgnore,
        850.f,
        MouseHitActor->GetActorLocation());

    int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);

    ULKAbilitySystemLibrary::GetClosestTargets(
        NumAdditionalTargets,
        OverlappingActors,
        OutAdditionalTargets,
        MouseHitActor->GetActorLocation());

    for (AActor* Target : OutAdditionalTargets)
    {
        if (ILKCombatInterface* CombatInterface = Cast<ILKCombatInterface>(Target))
        {
            if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ULKBeamSpell::AdditionalTargetDied))
            {
                CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ULKBeamSpell::AdditionalTargetDied);
            }
        }
    }
}
