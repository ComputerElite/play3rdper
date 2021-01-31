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