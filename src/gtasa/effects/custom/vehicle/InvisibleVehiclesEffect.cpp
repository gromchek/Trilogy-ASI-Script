#include "util/EffectBase.h"
#include "util/hooks/HookMacros.h"

#include <CBike.h>
#include <CBoat.h>
#include <CModelInfo.h>
#include <CPlane.h>
#include <CShadows.h>
#include <CTrain.h>

using namespace plugin;

class InvisibleVehiclesEffect : public EffectBase
{
    bool oldForceVehicleLightsOff = false;

public:
    void
    OnStart (EffectInstance *inst) override
    {
        oldForceVehicleLightsOff = CVehicle::ms_forceVehicleLightsOff;

        HOOK (inst, Hooked_RwIm3DTransform,
              uint8_t * (uint8_t *, signed int, RwMatrix *, unsigned int),
              0x6F07C7, 0x6F08E1);

        HOOK_METHOD_ARGS (inst, Hooked_RenderEffects, void (CEntity *),
                          0x5343B2);

        // CAutomobile::Render
        HOOK_METHOD (inst, Hooked_Empty, void (CAutomobile *), 0x6A2C24);

        // CBike::Render
        HOOK_METHOD (inst, Hooked_Empty, void (CBike *), 0x6BDE5E);

        // CPlane::Render
        HOOK_METHOD (inst, Hooked_Empty, void (CPlane *), 0x6CAB80);

        // CHeli::Render
        HOOK_METHOD (inst, Hooked_Empty, void (CHeli *), 0x6C4523);

        // CTrain::Render
        HOOK_METHOD (inst, Hooked_Empty, void (CTrain *), 0x6F55C0);

        // CBoat::Render
        HOOK_METHOD (inst, Hooked_Empty, void (CBoat *), 0x6F022F);

        // Vehicle Headlights (Car, Bike)
        // CVehicle::DoHeadLightBeam
        HOOK_METHOD (inst, Hooked_Empty,
                     void (CVehicle *, int, CMatrix *, char), 0x6A2EDA,
                     0x6A2EF2, 0x6BDE80);

        // Vehicle Exhaust Particles (Car, Bike)
        // CVehicle::AddExhaustParticles
        HOOK_METHOD (inst, Hooked_Empty, void (CVehicle *), 0x6AB344, 0x6BD3FF);

        // Vehicle Water Splash Particles (Car)
        // CVehicle::AddWaterSplashParticles
        HOOK_METHOD (inst, Hooked_Empty, void (CVehicle *), 0x6AB2FF);

        // Vehicle Wheel Particles (Car, Bike)
        // CVehicle::AddSingleWheelParticles
        HOOK_METHOD (inst, Hooked_Empty,
                     void (CVehicle *, int, int, float, float, CColPoint *,
                           RwV3d *, int, signed int, int, int, char),
                     0x6AB136, 0x6AB1FD, 0x6AB2B7, 0x6BD38C, 0x6C0AFD);

        // Vehicle Damage Particles (Car, Bike)
        // CVehicle::AddDamagedVehicleParticles
        HOOK_METHOD (inst, Hooked_Empty, void (CVehicle *), 0x6AB34B, 0x6BD40A);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        CVehicle::ms_forceVehicleLightsOff = oldForceVehicleLightsOff;
    }

    void
    OnTick (EffectInstance *inst) override
    {
        CVehicle::ms_forceVehicleLightsOff = true;
    }

    // --------------- Hooks ---------------
    static uint8_t *
    Hooked_RwIm3DTransform (auto &&cb)
    {
        return nullptr;
    }

    static void
    Hooked_RenderEffects (auto &&cb, CEntity *thisEntity)
    {
        if (!thisEntity) return;

        if (!CModelInfo::IsVehicleModelType (thisEntity->m_nModelIndex))
        {
            cb ();
        }
    }

    static void
    Hooked_Empty (auto &&cb)
    {
    }
};

DEFINE_EFFECT (InvisibleVehiclesEffect, "effect_invisible_vehicles",
               GROUP_INVISIBLE_VEHICLES | GROUP_VEHICLE_COLOR);