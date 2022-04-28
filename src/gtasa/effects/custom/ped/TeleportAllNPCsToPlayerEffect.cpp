#include "util/EffectBase.h"

class TeleportAllNPCsToPlayerEffect : public EffectBase
{
public:
    bool
    CanActivate () override
    {
        return FindPlayerPed ();
    }

    void
    OnTick (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (player)
        {
            CVector position = player->GetPosition ();

            for (CPed *ped : CPools::ms_pPedPool)
            {
                if (ped == player || ped->m_pVehicle) continue;

                ped->SetPosn (position
                              + CVector (inst->Random (-1.0f, 1.0f),
                                         inst->Random (-1.0f, 1.0f), 3.0f));
            }

            inst->Disable ();
        }
    }
};

DEFINE_EFFECT (TeleportAllNPCsToPlayerEffect,
               "effect_teleport_all_npcs_to_player", 0);