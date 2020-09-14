#pragma once

#include "BattleRoyal.h"
#include "Engine/GameInstance.h"
#include "BRGameInstance.generated.h"

UCLASS()
class BATTLEROYAL_API UBRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UBRGameInstance();
    
    FORCEINLINE USkeletalMesh* GetSkin() { return Skin; }
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    USkeletalMesh* Skin;
};
