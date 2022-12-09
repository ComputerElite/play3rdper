#pragma once
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(Active, bool, "Active", true);
    CONFIG_VALUE(Fixed, bool, "Fixed camera position", false);
    CONFIG_VALUE(LeftSaber, bool, "camera to saber", false);
    CONFIG_VALUE(SwapSaber, bool, "Swap saber", false);
    CONFIG_VALUE(ApplyOffsets, bool, "Also apply offsets", false);
    CONFIG_VALUE(OnlyPos, bool, "Only apply position", false);
    CONFIG_VALUE(MoveWhilePlaying, bool, "Move camera while playing", true);
    CONFIG_VALUE(DisableWhileReplay, bool, "Disable While Replay", true);
    CONFIG_VALUE(WashingMachine, bool, "Spin camera", false);
    CONFIG_VALUE(WashingMachineSpeed, float, "Spinning speed", 0);
    CONFIG_VALUE(XOffset, float, "X Offset", 2.0);
    CONFIG_VALUE(YOffset, float, "Y Offset", 1.0);
    CONFIG_VALUE(ZOffset, float, "Z Offset", -2.0);
    CONFIG_VALUE(XRot, float, "X Rot", 0);
    CONFIG_VALUE(YRot, float, "Y Rot", 0);
    CONFIG_VALUE(ZRot, float, "Z Rot", 0);
    CONFIG_VALUE(MoveMultiplier, float, "Change movement multipler", 5.0f);
    CONFIG_VALUE(MoveController, int, "Controls for moving the camera", 0);
)