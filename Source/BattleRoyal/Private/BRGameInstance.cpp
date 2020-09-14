#include "BRGameInstance.h"

UBRGameInstance::UBRGameInstance()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKIN(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith"));
    if (SKIN.Succeeded())
        Skin = SKIN.Object;
}
