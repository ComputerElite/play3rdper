#include "main.hpp"
#include "extern/config-utils/shared/config-utils.hpp"
#include "ModConfig.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/RelativeScoreAndImmediateRankCounter.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/PlayerTransforms.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/OVRPlugin_OVRP_1_1_0.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"

#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"

#include "CrashModViewController.hpp"

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace QuestUI;

static ModInfo modInfo;
DEFINE_CONFIG(ModConfig);

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

void Crash() {
    getModConfig().CrashCounter.SetValue(getModConfig().CrashCounter.GetValue() + 1);
    getLogger().info("Crash number %d by Crash mod", getModConfig().CrashCounter.GetValue());
    if(getModConfig().OnCrashAction.GetValue() == 0) CRASH_UNLESS(false);
    else if (getModConfig().OnCrashAction.GetValue() == 1) Application::Quit();
    else if (getModConfig().OnCrashAction.GetValue() == 2) while (true) malloc(1024);
}

MAKE_HOOK_MATCH(ScoreController_HandleNoteWasMissed, &ScoreController::HandleNoteWasMissed, void, ScoreController* self, NoteController* note) {
    ScoreController_HandleNoteWasMissed(self, note);
    if(getModConfig().Active.GetValue() && getModConfig().MissCrash.GetValue()) Crash();
}

MAKE_HOOK_MATCH(ScoreController_HandleNoteWasCut, &ScoreController::HandleNoteWasCut, void, ScoreController* self, NoteController* note, NoteCutInfo& info) {
    ScoreController_HandleNoteWasCut(self, note, info);
    if(getModConfig().Active.GetValue() && info.get_allIsOK() && getModConfig().CrashOnGoodCut.GetValue()) Crash();
}

MAKE_HOOK_MATCH(RelativeScoreAndImmediateRankCounter_UpdateRelativeScoreAndImmediateRank, &RelativeScoreAndImmediateRankCounter::UpdateRelativeScoreAndImmediateRank, void, RelativeScoreAndImmediateRankCounter* self, int score, int modifiedscore, int maxscore, int maxmodfifiedscore) {
    RelativeScoreAndImmediateRankCounter_UpdateRelativeScoreAndImmediateRank(self, score, modifiedscore, maxscore, maxmodfifiedscore);
    if(getModConfig().Active.GetValue() && getModConfig().PercentageActive.GetValue() && self->get_relativeScore() < getModConfig().Percentage.GetValue() / 100) Crash();
}

MAKE_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_Init, &StandardLevelScenesTransitionSetupDataSO::Init, void, StandardLevelScenesTransitionSetupDataSO* self, Il2CppString* gameMode, IDifficultyBeatmap* dbm, IPreviewBeatmapLevel* previewBeatmapLevel, OverrideEnvironmentSettings* overrideEnvironmentSettings, ColorScheme* overrideColorScheme, GameplayModifiers* gameplayModifiers, PlayerSpecificSettings* playerSpecificSettings, PracticeSettings* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {
    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, dbm, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
    if(getModConfig().Active.GetValue()) {
        if(getModConfig().CrashOnPlay.GetValue() || getModConfig().CrashOnNoFailOn.GetValue() && gameplayModifiers->noFailOn0Energy) Crash();
    }
}

MAKE_HOOK_MATCH(PlayerTransforms_Update, &PlayerTransforms::Update, void, PlayerTransforms* self) {
    PlayerTransforms_Update(self);
    if(getModConfig().Active.GetValue() && getModConfig().CrashOnTurn.GetValue() && self->headWorldRot.get_eulerAngles().y > 165 && self->headWorldRot.get_eulerAngles().y < 195) Crash();
}

MAKE_HOOK_MATCH(PauseController_HandleMenuButtonTriggered, &PauseController::HandleMenuButtonTriggered, void, PauseController* self) {
    PauseController_HandleMenuButtonTriggered(self);
    if(getModConfig().Active.GetValue() && getModConfig().CrashOnPause.GetValue()) Crash();
}

MAKE_HOOK_MATCH(PauseController_HandlePauseMenuManagerDidFinishResumeAnimation, &PauseController::HandlePauseMenuManagerDidFinishResumeAnimation, void, PauseController* self) {
    PauseController_HandlePauseMenuManagerDidFinishResumeAnimation(self);
    if(getModConfig().Active.GetValue() && getModConfig().CrashOnUnpause.GetValue()) Crash();
}

MAKE_HOOK_MATCH(SceneManager_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene nextActiveScene) {
    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
    if(getModConfig().Active.GetValue() && getModConfig().CrashOnOver5PerBattery.GetValue() && GlobalNamespace::OVRPlugin::OVRP_1_1_0::ovrp_GetSystemBatteryLevel() > getModConfig().BatteryThreshold.GetValue() / 100) Crash();
}

MAKE_HOOK_MATCH(ResultsViewController_Init, &ResultsViewController::Init, void, ResultsViewController* self, LevelCompletionResults* result, IDifficultyBeatmap* beatmap, bool practice, bool newHighScore) {
    ResultsViewController_Init(self, result, beatmap, practice, newHighScore);
    if(getModConfig().Active.GetValue() && ((getModConfig().CrashOnNotFullCombo.GetValue() && !result->fullCombo) || (getModConfig().CrashOnNewHighscore.GetValue() && self->newHighScore))) Crash();
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getLogger().info("Completed setup!");
}

extern "C" void load() {
    getLogger().info("Installing hooks...");
    il2cpp_functions::Init();
    getModConfig().Init(modInfo);
    QuestUI::Init();

    LoggerContextObject logger = getLogger().WithContext("load");
    //custom_types::Register::RegisterType<CrashMod::CrashModViewController>();
    // Register our mod settings menu
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    // Install our hooks
    INSTALL_HOOK(logger, PlayerTransforms_Update);
    INSTALL_HOOK(logger, ScoreController_HandleNoteWasMissed);
    INSTALL_HOOK(logger, ScoreController_HandleNoteWasCut);
    INSTALL_HOOK(logger, RelativeScoreAndImmediateRankCounter_UpdateRelativeScoreAndImmediateRank);
    INSTALL_HOOK(logger, StandardLevelScenesTransitionSetupDataSO_Init);
    INSTALL_HOOK(logger, SceneManager_ActiveSceneChanged);
    INSTALL_HOOK(logger, ResultsViewController_Init);
    //INSTALL_HOOK_OFFSETLESS(logger, PauseController_HandleMenuButtonTriggered, il2cpp_utils::FindMethodUnsafe("", "PauseController", "HandleMenuButtonTriggered", 0));
    //INSTALL_HOOK_OFFSETLESS(logger, PauseController_HandlePauseMenuManagerDidFinishResumeAnimation, il2cpp_utils::FindMethodUnsafe("", "PauseController", "HandlePauseMenuManagerDidFinishResumeAnimation", 0));
    getLogger().info("Installed all hooks!");
}