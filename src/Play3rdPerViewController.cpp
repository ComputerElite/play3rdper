#include "Play3rdPerViewController.hpp"
#include "ModConfig.hpp"
#include "config-utils/shared/config-utils.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "main.hpp"

#include <stdlib.h>

using namespace BSML::Lite;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;


void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = CreateScrollableSettingsContainer(self->get_transform());

        // Active
        AddConfigValueToggle(container->get_transform(), getModConfig().Active);

        // Fixed
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().Fixed)->get_gameObject(), "Can make you dizzy and motion sick");

        // Saber
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().LeftSaber)->get_gameObject(), "Will most likely make you motion sick. fixes the camera to the left saber");

        // SwapSaber
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().SwapSaber)->get_gameObject(), "Will swap the saber the camera is attached to");

        // ApplyOffsets
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().ApplyOffsets)->get_gameObject(), "Wether to also add the offsets from below if the camera is attached to a saber");

        // OnlyPos
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().OnlyPos)->get_gameObject(), "only the position gets applied to the camera and NOT the Rotation");

        // MoveWhilePlaying
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().MoveWhilePlaying)->get_gameObject(), "If this is activated you can move the camera while playing if you press Y");
        
        // MoveMultiplier
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().MoveMultiplier, 1, 1.0f, -100, 100)->get_gameObject(), "If you hold the Y button while moving your head, you can adjust the camera position while playing. This multiplier decides how fast you move the camera.");

        // WashingMachine
        AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().WashingMachine)->get_gameObject(), "Wanna rotate a lot? Here ya go");
        
        // WashingMachineSpeed
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().WashingMachineSpeed, 1, 0.5f, -100, 100)->get_gameObject(), "Speed at which you are rotating");

        /*
        Enable later once replay is out
        // DisableWhileReplay
        auto DisableWhileReplayPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetDisableWhileReplay);
        UnityEngine::UI::Toggle* DisableWhileReplayObject = CreateToggle(container->get_transform(), "Disable while viewing Replays", getConfig().config["DisableWhileReplay"].GetBool(), DisableWhileReplayPress);
        AddHoverHint(DisableWhileReplayObject->get_gameObject(), "While viewing a replay, the Replay mod has it's own camera control system! Enabling this setting will result in Third person mod being disabled while you view a replay.");
        */

        // X
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().XOffset, 1, 0.1f, -100, 100)->get_gameObject(), "Offset on the x coordinate");

        // Y
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().YOffset, 1, 0.1f, -100, 100)->get_gameObject(), "Offset on the y coordinate");

        // Z
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().ZOffset, 1, 0.1f, -100, 100)->get_gameObject(), "Offset on the z coordinate");


        // XR
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().XRot, 0, 5.0f, -360, 360)->get_gameObject(), "Offset on the x rotation");

        // YR
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().YRot, 0, 5.0f, -360, 360)->get_gameObject(), "Offset on the y rotation");

        // ZR
        AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().ZRot, 0, 5.0f, -360, 360)->get_gameObject(), "Offset on the z rotation");

        // MoveController
        AddHoverHint(AddConfigValueIncrementInt(container->get_transform(), getModConfig().MoveController, 1, 0, 2)->get_gameObject(), "What is used to move the camera (0 = Head, 1 = Left Controller, 2 = Right Controller)");
    }
}