#include "main.hpp"
#include "GlobalNamespace/LightManager.hpp"
#include "GlobalNamespace/MainCamera.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/PlayerTransforms.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/FireworksController.hpp"
#include "GlobalNamespace/NoteCutCoreEffectsSpawner.hpp"
#include "GlobalNamespace/ShockwaveEffect.hpp"
#include "GlobalNamespace/SaberTrail.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/GameObject.hpp"

#include "Play3rdPerViewController.hpp"

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"

using namespace GlobalNamespace;

GameplayModifiers* gpm = nullptr;

static ModInfo modInfo;
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

MAKE_HOOK_OFFSETLESS(LightManager_OnWillRenderObject, void, LightManager* self, UnityEngine::Camera* camera) {
  // Do stuff when this function is called 
  LightManager_OnWillRenderObject(self, camera); 
  if(!(getConfig().config["Active"].GetBool())) return;

  UnityEngine::Camera* c = UnityEngine::Camera::get_main();
  UnityEngine::Vector3 rot = c->get_transform()->get_eulerAngles();
  UnityEngine::Vector3 pos = c->get_transform()->get_position();

  pos.x += getConfig().config["XOffset"].GetFloat();
  pos.y += getConfig().config["YOffset"].GetFloat();
  pos.z += getConfig().config["ZOffset"].GetFloat();

  rot.x += getConfig().config["XRot"].GetFloat();
  rot.y += getConfig().config["YRot"].GetFloat();
  rot.z += getConfig().config["ZRot"].GetFloat();
  

  c->get_transform()->set_position(pos);
  c->get_transform()->set_eulerAngles(rot);
  
}

void createDefaultConfig()  {

    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("XRot"))) {
        getConfig().config.AddMember("XRot", rapidjson::Value().SetFloat(0), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("YRot"))) {
        getConfig().config.AddMember("YRot", rapidjson::Value().SetFloat(0), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("ZRot"))) {
        getConfig().config.AddMember("ZRot", rapidjson::Value().SetFloat(0), allocator);
    }

    if(getConfig().config.HasMember("Active")) {return;}

    // Add all the default options
    getConfig().config.RemoveAllMembers(); // Empty the config - it should already be empty but just to be sure
    getConfig().config.SetObject(); // Set the base of the config to a value that can contain keys

    // Get what is used to allocate memory in the config file
    // rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();

    // Add a member to the config, using the allocator

    getConfig().config.AddMember("Active", rapidjson::Value().SetBool(true), allocator);
    getConfig().config.AddMember("XOffset", rapidjson::Value().SetFloat(2.0), allocator);
    getConfig().config.AddMember("YOffset", rapidjson::Value().SetFloat(1.0), allocator);
    getConfig().config.AddMember("ZOffset", rapidjson::Value().SetFloat(-2.0), allocator);
    getConfig().config.AddMember("XRot", rapidjson::Value().SetFloat(0), allocator);
    getConfig().config.AddMember("YRot", rapidjson::Value().SetFloat(0), allocator);
    getConfig().config.AddMember("ZRot", rapidjson::Value().SetFloat(0), allocator);

    getConfig().Write(); // Write the config back to disk
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    createDefaultConfig();
    getLogger().info("Completed setup!");
}

extern "C" void load() {
    getLogger().info("Installing hooks...");
    il2cpp_functions::Init();
    QuestUI::Init();
custom_types::Register::RegisterType<Play3rdPer::Play3rdPerViewController>();
QuestUI::Register::RegisterModSettingsViewController<Play3rdPer::Play3rdPerViewController*>(modInfo);
    // Install our hooks
    LoggerContextObject logger = getLogger().WithContext("load");
    INSTALL_HOOK_OFFSETLESS(logger, LightManager_OnWillRenderObject, il2cpp_utils::FindMethodUnsafe("", "LightManager", "OnWillRenderObject", 0));

    getLogger().info("Installed all hooks!");
}