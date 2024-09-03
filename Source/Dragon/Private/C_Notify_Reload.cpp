#include "C_Notify_Reload.h"
#include "CHelpers.h"

#include "C_WeaponComponent.h"

FString UC_Notify_Reload::GetNotifyName_Implementation() const
{
	return "Reload";
}

void UC_Notify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNUll(MeshComp);
	CheckNUll(MeshComp->GetOwner());

	UC_WeaponComponent* weapon = Cast<UC_WeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_WeaponComponent::StaticClass()));
	CheckNUll(weapon);

	switch (ActionType)
	{
	case EReloadType::Eject:
		weapon->Eject_Magazine();
		break;
	case EReloadType::Spawn:
		weapon->Spawn_Magazine();
		break;
	case EReloadType::Load:
		weapon->Load_Magazine();
		break;
	case EReloadType::End:
		weapon->End_Reload();
		break;
	}
}
