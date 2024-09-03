#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Magazine.generated.h"

UCLASS()
class DRAGON_API AC_Magazine : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh_Full;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh_Empty;

public:	
	AC_Magazine();

protected:
	virtual void BeginPlay() override;

public:
	void SetEject();

private:
	bool bEject;
};
