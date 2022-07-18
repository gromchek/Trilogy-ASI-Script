#include "util/EffectBase.h"
#include "util/GameUtil.h"

#include <CCheat.h>

class DeathEffect : public EffectBase
{
public:
    bool
    CanActivate () override
    {
        return GameUtil::IsPlayerSafe ();
    }

    void
    OnTick (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player || !CanActivate ())
        {
            inst->ResetTimer ();
            return;
        }

        CCheat::SuicideCheat ();
        inst->Disable ();
    }
};

DEFINE_EFFECT (DeathEffect, "effect_death", GROUP_HEALTH);