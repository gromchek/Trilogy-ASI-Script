#include "util/EffectBase.h"

#include <CAEAudioHardware.h>

using namespace plugin;

class HighPitchedAudioEffect : public EffectBase
{
    static inline float audioPitch = 1.5f;

public:
    void
    OnStart (EffectInstance *inst) override
    {
        HOOK_METHOD (inst, Hooked_SetFrequencyScalingFactor,
                     int (CAEAudioHardware *, int, int, float), 0x4D6E34,
                     0x4D6E48, 0x4DBF9B, 0x4EA62D, 0x4F0871, 0x4F0A58);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        injector::WriteMemory (0x8CBA6C, 1.0f, true);
    }

    void
    OnTick (EffectInstance *inst) override
    {
        injector::WriteMemory (0x8CBA6C, audioPitch, true);
    }

    // CAEAudioHardware *thisAudioHardware
    // int slot
    // int offset
    // float factor
    static int
    Hooked_SetFrequencyScalingFactor (auto &&SetFrequencyScalingFactor)
    {
        float &factor = std::get<3> (SetFrequencyScalingFactor.params);

        if (factor > 0.0f) factor = audioPitch;

        return SetFrequencyScalingFactor ();
    }
};

DEFINE_EFFECT (HighPitchedAudioEffect, "effect_high_pitched_audio",
               GROUP_AUDIO_PITCH);