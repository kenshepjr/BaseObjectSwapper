#include "Hooks.h"

namespace BaseObjectSwapper
{
    void detail::swap_base(RE::TESObjectREFR* a_ref)
    {
        // Exporter build:
        // Do not mutate references during normal initialization.
        // FullScan will evaluate all available references at kDataLoaded.
        (void)a_ref;
    }

    void Install()
    {
        REX::INFO("{:*^30}", "HOOKS");

        InitItemImpl<RE::TESObjectREFR>::Install();
        InitItemImpl<RE::Hazard>::Install();
        InitItemImpl<RE::ArrowProjectile>::Install();

        SetObjectReference<RE::TESObjectREFR>::Install();
        SetObjectReference<RE::Hazard>::Install();
        SetObjectReference<RE::ArrowProjectile>::Install();
    }
}
