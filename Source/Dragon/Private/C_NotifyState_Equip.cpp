#include "C_NotifyState_Equip.h"
#include "CHelpers.h"

#include "C_WeaponComponent.h"


FString UC_NotifyState_Equip::GetNotifyName_Implementation() const
{
    return "Equip";
}

void UC_NotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    //MeshComp와 MeshComp의 Owner가 유효한지 확인
    CheckNUll(MeshComp);
    CheckNUll(MeshComp->GetOwner());

    //Owner가 WeaponComponent를 가지고 있는지 확인
    UC_WeaponComponent* weapon = Cast<UC_WeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_WeaponComponent::StaticClass()));
    CheckNUll(weapon);

    //Owner가 WeaponComponent가 있으면 실행
    weapon->Begin_Equip();
}

void UC_NotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    //MeshComp와 MeshComp의 Owner가 유효한지 확인
    CheckNUll(MeshComp);
    CheckNUll(MeshComp->GetOwner());

    //Owner가 WeaponComponent를 가지고 있는지 확인
    UC_WeaponComponent* weapon = Cast<UC_WeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_WeaponComponent::StaticClass()));
    CheckNUll(weapon);

    weapon->End_Equip();
}
