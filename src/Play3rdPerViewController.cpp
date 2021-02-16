#include "Play3rdPerViewController.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include <stdlib.h>

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
using namespace Play3rdPer;

DEFINE_CLASS(Play3rdPerViewController);

void SetActive(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["Active"].SetBool(newValue);
}

void SetFixed(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["Fixed"].SetBool(newValue);
}

void SetSaber(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["LeftSaber"].SetBool(newValue);
}

void SetSwapSaber(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["SwapSaber"].SetBool(newValue);
}

void SetApplyOffsets(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["ApplyOffsets"].SetBool(newValue);
}

void SetOnlyPos(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["OnlyPos"].SetBool(newValue);
}

void SetMoveWhilePlaying(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["MoveWhilePlaying"].SetBool(newValue);
}

void SetDisableWhileReplay(Play3rdPerViewController* self, bool newValue) {
    getConfig().config["DisableWhileReplay"].SetBool(newValue);
}

void SetXFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["XOffset"].SetFloat(newValue);
}
void SetYFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["YOffset"].SetFloat(newValue);
}
void SetZFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["ZOffset"].SetFloat(newValue);
}

void SetXRFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["XRot"].SetFloat(newValue);
}
void SetYRFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["YRot"].SetFloat(newValue);
}
void SetZRFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["ZRot"].SetFloat(newValue);
}

void SetMoveMultiplierFloat(Play3rdPerViewController* self, float newValue)   {
    getConfig().config["MoveMultiplier"].SetFloat(newValue);
}


void Play3rdPerViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation) {
        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

        // Active
        auto ActivePress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetActive);
        QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Active", getConfig().config["Active"].GetBool(), ActivePress);

        // Fixed
        auto FixedPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetFixed);
        UnityEngine::UI::Toggle* FixedObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Fixed camera position", getConfig().config["Fixed"].GetBool(), FixedPress);
        QuestUI::BeatSaberUI::AddHoverHint(FixedObject->get_gameObject(), "Can make you dizzy and motion sick");

        // Saber
        auto SaberPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetSaber);
        UnityEngine::UI::Toggle* SaberObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "camera to saber", getConfig().config["LeftSaber"].GetBool(), SaberPress);
        QuestUI::BeatSaberUI::AddHoverHint(SaberObject->get_gameObject(), "Will most likely make you motion sick. fixes the camera to the left saber");

        // SwapSaber
        auto SwapSaberPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetSwapSaber);
        UnityEngine::UI::Toggle* SwapSaberObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Swap saber", getConfig().config["SwapSaber"].GetBool(), SwapSaberPress);
        QuestUI::BeatSaberUI::AddHoverHint(SwapSaberObject->get_gameObject(), "Will swap the saber the camera is attached to");

        // ApplyOffsets
        auto ApplyOffsetsPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetApplyOffsets);
        UnityEngine::UI::Toggle* ApplyOffsetsObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Also apply offsets", getConfig().config["ApplyOffsets"].GetBool(), ApplyOffsetsPress);
        QuestUI::BeatSaberUI::AddHoverHint(ApplyOffsetsObject->get_gameObject(), "Wether to also add the offsets from below if the camera is attached to a saber");

        // OnlyPos
        auto OnlyPosPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetOnlyPos);
        UnityEngine::UI::Toggle* OnlyPosObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Only apply position", getConfig().config["OnlyPos"].GetBool(), OnlyPosPress);
        QuestUI::BeatSaberUI::AddHoverHint(OnlyPosObject->get_gameObject(), "only the position gets applied to the camera and NOT the Rotation");

        // MoveWhilePlaying
        auto MoveWhilePlayingPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetMoveWhilePlaying);
        UnityEngine::UI::Toggle* MoveWhilePlayingObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Move camera while playing", getConfig().config["MoveWhilePlaying"].GetBool(), MoveWhilePlayingPress);
        QuestUI::BeatSaberUI::AddHoverHint(MoveWhilePlayingObject->get_gameObject(), "If this is activated you can move the camera while playing if you press Y");
        
        // MoveMultiplier
        auto MoveMultiplierChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetMoveMultiplierFloat);
        QuestUI::IncrementSetting* MoveMultiplierObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Change movement multipler", 1, 0.1, getConfig().config["MoveMultiplier"].GetFloat(), 0, 100, MoveMultiplierChange);
        QuestUI::BeatSaberUI::AddHoverHint(MoveMultiplierObject->get_gameObject(), "If you hold the Y button while moving your head, you can adjust the camera position while playing. This multiplier decides how fast you move the camera.");

        // DisableWhileReplay
        auto DisableWhileReplayPress = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, SetDisableWhileReplay);
        UnityEngine::UI::Toggle* DisableWhileReplayObject = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Disable while viewing Replays", getConfig().config["DisableWhileReplay"].GetBool(), DisableWhileReplayPress);
        QuestUI::BeatSaberUI::AddHoverHint(DisableWhileReplayObject->get_gameObject(), "While viewing a replay, the Replay mod has it's own camera control system! Enabling this setting will result in Third person mod being disabled while you view a replay.");

        // X
        auto XChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetXFloat);
        QuestUI::IncrementSetting* XObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "X offset", 1, 0.1, getConfig().config["XOffset"].GetFloat(), -100.0f, 100.0f, XChange);
        QuestUI::BeatSaberUI::AddHoverHint(XObject->get_gameObject(), "Offset on the x coordinate");

        // Y
        auto YChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetYFloat);
        QuestUI::IncrementSetting* YObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Y offset", 1, 0.1, getConfig().config["YOffset"].GetFloat(), -100.0f, 100.0f, YChange);
        QuestUI::BeatSaberUI::AddHoverHint(YObject->get_gameObject(), "Offset on the y coordinate");

        // Z
        auto ZChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetZFloat);
        QuestUI::IncrementSetting* ZObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Z offset", 1, 0.1, getConfig().config["ZOffset"].GetFloat(), -100.0f, 100.0f, ZChange);
        QuestUI::BeatSaberUI::AddHoverHint(ZObject->get_gameObject(), "Offset on the z coordinate");


        // XR
        auto XRChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetXRFloat);
        QuestUI::IncrementSetting* XRObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "X Rotation", 0, 5.0, getConfig().config["XRot"].GetFloat(), -360.0f, 360.0f, XRChange);
        QuestUI::BeatSaberUI::AddHoverHint(XRObject->get_gameObject(), "Offset on the x rotation");

        // YR
        auto YRChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetYRFloat);
        QuestUI::IncrementSetting* YRObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Y Rotation", 0, 5.0, getConfig().config["YRot"].GetFloat(), -360.0f, 360.0f, YRChange);
        QuestUI::BeatSaberUI::AddHoverHint(YRObject->get_gameObject(), "Offset on the y rotation");

        // ZR
        auto ZRChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
                    classof(UnityEngine::Events::UnityAction_1<float>*), this, SetZRFloat);
        QuestUI::IncrementSetting* ZRObject = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Z Rotation", 0, 5.0, getConfig().config["ZRot"].GetFloat(), -360.0f, 360.0f, ZRChange);
        QuestUI::BeatSaberUI::AddHoverHint(ZRObject->get_gameObject(), "Offset on the z rotation");
    }
}

void Play3rdPerViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
    getConfig().Write();
}