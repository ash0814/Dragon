#include "C_W_AK47.h"
#include "CHelpers.h"
#include "C_Magazine.h"

AC_W_AK47::AC_W_AK47()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Project/Asset/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (SkelMesh.Succeeded())
		Mesh->SetSkeletalMesh(SkelMesh.Object);

	//Equip
	{
		HolsterSocketName = "Holster_Rifle";
		RightSocketName = "Weapon_R";

		static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim(TEXT("/Script/Engine.AnimMontage'/Game/Project/Asset/AnimStarterPack/Montages/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'"));
		if (Anim.Succeeded())
			EquipMontage = Anim.Object;

		EquipMontage_PlayRate = 2.0f;
		LeftHandLocation = FVector(-37.0f, 12.5f, 7.0f);
	}

	//Reload
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> Reload(TEXT("/Script/Engine.AnimMontage'/Game/Project/Asset/AnimStarterPack/AnimStarter/Reload_Rifle.Reload_Rifle'"));
		if (Reload.Succeeded())
			ReloadMontage = Reload.Object;

		static ConstructorHelpers::FClassFinder<AC_Magazine> Mag(TEXT("/Script/Engine.Blueprint'/Game/Player/P_BulePrint/BP_C_Magazine.BP_C_Magazine_C'"));
		if (Mag.Succeeded())
			MagazineClass = Mag.Class;

		ReloadMontage_PlayRate = 1.0f;
		MagazineSocketName = "Magazine_L";
		MagazineBoneName = "b_gun_mag";
	}

	//Recoil
	{
		RecoilRate = 0.3f;
		RecoilAngle = 1.0f;
		FireInterval = 0.1f;
	}

	//Bullet
	{
		DefaultBulletCount = 40;
	}
}

