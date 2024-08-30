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

    //MeshComp�� MeshComp�� Owner�� ��ȿ���� Ȯ��
    CheckNUll(MeshComp);
    CheckNUll(MeshComp->GetOwner());

    //Owner�� WeaponComponent�� ������ �ִ��� Ȯ��
    UC_WeaponComponent* weapon = Cast<UC_WeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_WeaponComponent::StaticClass()));
    CheckNUll(weapon);

    //Owner�� WeaponComponent�� ������ ����
    weapon->Begin_Equip();
}

void UC_NotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    //MeshComp�� MeshComp�� Owner�� ��ȿ���� Ȯ��
    CheckNUll(MeshComp);
    CheckNUll(MeshComp->GetOwner());

    //Owner�� WeaponComponent�� ������ �ִ��� Ȯ��
    UC_WeaponComponent* weapon = Cast<UC_WeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_WeaponComponent::StaticClass()));
    CheckNUll(weapon);

    weapon->End_Equip();
}
