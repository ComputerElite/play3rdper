#include "CrashModViewController.hpp"
#include "ModConfig.hpp"
#include "extern/config-utils/shared/config-utils.hpp"

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
//using namespace CrashMod;

//DEFINE_CLASS(CrashModViewController);


void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        // Active
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().Active)->get_gameObject(), "Why would you want to disable it at all?");

        // PercentageActive
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().PercentageActive)->get_gameObject(), "Enabled the Crash for percentage");

        // Percentage;
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().Percentage, 0, 1.0f, 0.0f, 100.0f)->get_gameObject(), "Under this percentage (score) your game will crash.");

        // MissCrash
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().MissCrash)->get_gameObject(), "Crashes the game on a miss");

        // CrashOnGoodCut
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnGoodCut)->get_gameObject(), "Well it crashes the game if you do a good cut");

        // CrashOnPlay
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnPlay)->get_gameObject(), "Crashes the game when you start a level");

        // CrashOnTurn
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnTurn)->get_gameObject(), "Crashes the game when you turn around in a song");

        // CrashOnNoFailOn
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnNoFailOn)->get_gameObject(), "No fail is a great feature (especially for torture). So why don't just let you not do it at all?");

        // CrashOnNotFullCombo
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnNotFullCombo)->get_gameObject(), "Do a full combo or crash");

        // CrashOnNewHighscore
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnNewHighscore)->get_gameObject(), "Why would you want new high scores??? Don't get better");

        // BatteryThreshold
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueIncrementFloat(container->get_transform(), getModConfig().BatteryThreshold, 0, 1.0f, 0.0f, 100.0f)->get_gameObject(), "Over this percentage your game will crash.");

        // CrashOnOver5PerBattery
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().CrashOnOver5PerBattery)->get_gameObject(), "You'll have to play Beat Saber while your Quest dies");

        // CrashOnPause
        /*
        auto CrashOnPauseChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, onCrashOnPauseChange);
        UnityEngine::UI::Toggle* CrashOnPause = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Crash on Pause", getConfig().config["CrashOnPause"].GetBool(), CrashOnPauseChange);
        QuestUI::BeatSaberUI::AddHoverHint(CrashOnPause->get_gameObject(), "Crashes the game when you pause it");

        // CrashOnUnpause
        auto CrashOnUnpauseChange = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
                   classof(UnityEngine::Events::UnityAction_1<bool>*), this, onCrashOnUnpauseChange);
        UnityEngine::UI::Toggle* CrashOnUnpause = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Crash on Resume", getConfig().config["CrashOnUnpause"].GetBool(), CrashOnUnpauseChange);
        QuestUI::BeatSaberUI::AddHoverHint(CrashOnUnpause->get_gameObject(), "Crashes the game when you resume.");
        */
       
        // OnCrashAction
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueIncrementInt(container->get_transform(), getModConfig().OnCrashAction, 1, 0, 1)->get_gameObject(), "Why would you even want to do that? (0 = Crash, 1 = Quit)");

        // Crash Now
        BeatSaberUI::CreateUIButton(container->get_transform(), "Crash now", 
            []() {
                getModConfig().CrashCounter.SetValue(getModConfig().CrashCounter.GetValue() + 1);
                CRASH_UNLESS(false);
            });

        // Crash Counter
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "Crash Count: " + std::to_string(getModConfig().CrashCounter.GetValue()));
    }
}