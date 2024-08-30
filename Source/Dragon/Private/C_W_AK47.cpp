#include "C_W_AK47.h"
#include "CHelpers.h"

AC_W_AK47::AC_W_AK47()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Project/Asset/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));

	if (SkelMesh.Succeeded())
		Mesh->SetSkeletalMesh(SkelMesh.Object);

	//Equip
	{
		HolsterSocketName = "Holster_Rifle";

		static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim(TEXT("/Script/Engine.AnimMontage'/Game/Project/Asset/AnimStarterPack/Montages/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
		if (Anim.Succeeded())
			EquipMontage = Anim.Object;

		EquipMontage_PlayRate = 2.0f;
		RightSocketName = "Weapon_R";
		LeftHandLocation = FVector(-32.5f, 15.5f, 7.0f);
	}
}

