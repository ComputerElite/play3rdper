#pragma once
#include "extern/config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    DECLARE_VALUE(Active, bool, "Active", true);
    DECLARE_VALUE(Fixed, bool, "Fixed camera position", false);
    DECLARE_VALUE(LeftSaber, bool, "camera to saber", false);
    DECLARE_VALUE(SwapSaber, bool, "Swap saber", false);
    DECLARE_VALUE(ApplyOffsets, bool, "Also apply offsets", false);
    DECLARE_VALUE(OnlyPos, bool, "Only apply position", false);
    DECLARE_VALUE(MoveWhilePlaying, bool, "Move camera while playing", true);
    DECLARE_VALUE(DisableWhileReplay, bool, "Disable While Replay", true);
    DECLARE_VALUE(XOffset, float, "X Offset", 2.0);
    DECLARE_VALUE(YOffset, float, "Y Offset", 1.0);
    DECLARE_VALUE(ZOffset, float, "Z Offset", -2.0);
    DECLARE_VALUE(XRot, float, "X Rot", 0);
    DECLARE_VALUE(YRot, float, "Y Rot", 0);
    DECLARE_VALUE(ZRot, float, "Z Rot", 0);
    DECLARE_VALUE(MoveMultiplier, float, "Change movement multipler", 5.0f);

    INIT_FUNCTION(
        INIT_VALUE(Active);
        INIT_VALUE(Fixed);
        INIT_VALUE(LeftSaber);
        INIT_VALUE(SwapSaber);
        INIT_VALUE(ApplyOffsets);
        INIT_VALUE(OnlyPos);
        INIT_VALUE(MoveWhilePlaying);
        INIT_VALUE(DisableWhileReplay);
        INIT_VALUE(XOffset);
        INIT_VALUE(YOffset);
        INIT_VALUE(ZOffset);
        INIT_VALUE(XRot);
        INIT_VALUE(YRot);
        INIT_VALUE(ZRot);
        INIT_VALUE(MoveMultiplier);
    )
)