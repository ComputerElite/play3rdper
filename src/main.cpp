#include "main.hpp"
#include "GlobalNamespace/LightManager.hpp"
#include "GlobalNamespace/VRControllersInputManager.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "GlobalNamespace/AvatarPoseController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Matrix4x4.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "Play3rdPerViewController.hpp"

#include "questui/shared/QuestUI.hpp"
#include "custom-types/shared/register.hpp"

using namespace GlobalNamespace;

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

UnityEngine::Matrix4x4 TranslateMatrix(UnityEngine::Vector3 vector)
        {
            UnityEngine::Matrix4x4 result;
            result.m00 = 1;
            result.m01 = 0;
            result.m02 = 0;
            result.m03 = vector.x;
            result.m10 = 0;
            result.m11 = 1;
            result.m12 = 0;
            result.m13 = vector.y;
            result.m20 = 0;
            result.m21 = 0;
            result.m22 = 1;
            result.m23 = vector.z;
            result.m30 = 0;
            result.m31 = 0;
            result.m32 = 0;
            result.m33 = 1;
            return result;
        }

VRController* rightController = nullptr;
VRController* leftController = nullptr;
UnityEngine::Vector3 saberRot = UnityEngine::Vector3(getConfig().config["XRot"].GetFloat(), getConfig().config["YRot"].GetFloat(), getConfig().config["ZRot"].GetFloat());
UnityEngine::Vector3 saberPos = UnityEngine::Vector3(getConfig().config["XOffset"].GetFloat(), getConfig().config["YOffset"].GetFloat(), getConfig().config["ZOffset"].GetFloat());

UnityEngine::Vector3 prevPos = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
UnityEngine::Vector3 prevRot = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
// UnityEngine::Vector3 prevPosOffset = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
// UnityEngine::Vector3 prevRotOffset = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);

bool replay = false;

MAKE_HOOK_OFFSETLESS(LightManager_OnWillRenderObject, void, LightManager* self, UnityEngine::Camera* camera) {
  // Do stuff when this function is called 
  LightManager_OnWillRenderObject(self, camera); 
  if(!(getConfig().config["Active"].GetBool()) || replay) return;
  UnityEngine::Camera* c = UnityEngine::Camera::get_main();
  UnityEngine::Vector3 rot = c->get_transform()->get_eulerAngles();
  UnityEngine::Vector3 pos = c->get_transform()->get_position();

  if(getConfig().config["LeftSaber"].GetBool() && getConfig().config["SwapSaber"].GetBool() && rightController != nullptr) {
        saberPos = rightController->get_position();
        saberRot = rightController->get_rotation().get_eulerAngles();
    } else if(getConfig().config["LeftSaber"].GetBool() && leftController != nullptr) {
        saberPos = leftController->get_position();
        saberRot = leftController->get_rotation().get_eulerAngles();
    }

    if(getConfig().config["MoveWhilePlaying"].GetBool()) {
        GlobalNamespace::OVRInput::Update();
        if(GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Four, OVRInput::Controller::Touch)) {
            UnityEngine::Vector3 posDifference = pos - prevPos;
            getConfig().config["XOffset"].SetFloat(posDifference.x * getConfig().config["MoveMultiplier"].GetFloat() + getConfig().config["XOffset"].GetFloat());
            getConfig().config["YOffset"].SetFloat(posDifference.y * getConfig().config["MoveMultiplier"].GetFloat() + getConfig().config["YOffset"].GetFloat());
            getConfig().config["ZOffset"].SetFloat(posDifference.z * getConfig().config["MoveMultiplier"].GetFloat() + getConfig().config["ZOffset"].GetFloat());
        }
        prevPos = pos;
        prevRot = rot;
    }

    UnityEngine::Vector3 offsetRot = UnityEngine::Vector3(getConfig().config["XRot"].GetFloat(), getConfig().config["YRot"].GetFloat(), getConfig().config["ZRot"].GetFloat());
    UnityEngine::Vector3 offsetPos = UnityEngine::Vector3(getConfig().config["XOffset"].GetFloat(), getConfig().config["YOffset"].GetFloat(), getConfig().config["ZOffset"].GetFloat());
    
  typedef function_ptr_t<void, UnityEngine::Camera*, UnityEngine::Matrix4x4> type;
auto method = *reinterpret_cast<type>(il2cpp_functions::resolve_icall("UnityEngine.Camera::set_cullingMatrix_Injected"));

method(c, UnityEngine::Matrix4x4::Ortho(-99999, 99999, -99999, 99999, 0.001f, 99999) * TranslateMatrix(UnityEngine::Vector3::get_forward() * -99999 / 2) * c->get_worldToCameraMatrix());

  if(getConfig().config["Fixed"].GetBool()) {
    if(getConfig().config["LeftSaber"].GetBool()) { 
        pos.x = saberPos.x + (getConfig().config["ApplyOffsets"].GetBool() ? offsetPos.x : 0.0f);
        pos.y = saberPos.y + (getConfig().config["ApplyOffsets"].GetBool() ? offsetPos.y : 0.0f);
        pos.z = saberPos.z + (getConfig().config["ApplyOffsets"].GetBool() ? offsetPos.z : 0.0f);

        if(!getConfig().config["OnlyPos"].GetBool()) {
            rot.x = saberRot.x + (getConfig().config["ApplyOffsets"].GetBool() ? offsetRot.x : 0.0f);
            rot.y = saberRot.y + (getConfig().config["ApplyOffsets"].GetBool() ? offsetRot.y : 0.0f);
            rot.z = saberRot.z + (getConfig().config["ApplyOffsets"].GetBool() ? offsetRot.z : 0.0f);
        }
    } else {
        pos.x = offsetPos.x;
        pos.y = offsetPos.y;
        pos.z = offsetPos.z;

        if(!getConfig().config["OnlyPos"].GetBool()) {
            rot.x = offsetRot.z;
            rot.y = offsetRot.y;
            rot.z = offsetRot.z;
        }
    }
  } else {
      if(getConfig().config["LeftSaber"].GetBool()) {
        pos.x += saberPos.x + (getConfig().config["ApplyOffsets"].GetBool() ? offsetPos.x : 0.0f);
        pos.y += saberPos.y + (getConfig().config["ApplyOffsets"].GetBool() ? offsetPos.y : 0.0f);
        pos.z += saberPos.z + (getConfig().config["ApplyOffsets"].GetBool() ? offsetPos.z : 0.0f);

        if(!getConfig().config["OnlyPos"].GetBool()) {
            rot.x += saberRot.x + (getConfig().config["ApplyOffsets"].GetBool() ? offsetRot.x : 0.0f);
            rot.y += saberRot.y + (getConfig().config["ApplyOffsets"].GetBool() ? offsetRot.y : 0.0f);
            rot.z += saberRot.z + (getConfig().config["ApplyOffsets"].GetBool() ? offsetRot.z : 0.0f);
        }
      } else {
        pos.x += offsetPos.x;
        pos.y += offsetPos.y;
        pos.z += offsetPos.z;

        if(!getConfig().config["OnlyPos"].GetBool()) {
            rot.x += offsetRot.z;
            rot.y += offsetRot.y;
            rot.z += offsetRot.z;
        }
      }
  } 

  c->get_transform()->set_position(pos);
  c->get_transform()->set_eulerAngles(rot);
}

MAKE_HOOK_OFFSETLESS(AudioTimeSyncController_Start, void, AudioTimeSyncController* self) {
    AudioTimeSyncController_Start(self);
    if(getConfig().config["DisableWhileReplay"].GetBool() && (strcmp(getenv("ViewingReplay"), "true") == 0)) replay = true;
}

MAKE_HOOK_OFFSETLESS(SceneManager_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene nextActiveScene) {
    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
    auto controllers = UnityEngine::Resources::FindObjectsOfTypeAll<VRController*>();
    for (int i = 0; i < controllers->Length(); i++) {
        if(controllers->values[i]->get_node() == UnityEngine::XR::XRNode::LeftHand) {
            leftController = controllers->values[i];
        } else if(controllers->values[i]->get_node() == UnityEngine::XR::XRNode::RightHand){
            rightController = controllers->values[i];
        }
    }
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

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("Fixed"))) {
        getConfig().config.AddMember("Fixed", rapidjson::Value().SetBool(false), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("LeftSaber"))) {
        getConfig().config.AddMember("LeftSaber", rapidjson::Value().SetBool(false), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("SwapSaber"))) {
        getConfig().config.AddMember("SwapSaber", rapidjson::Value().SetBool(false), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("ApplyOffsets"))) {
        getConfig().config.AddMember("ApplyOffsets", rapidjson::Value().SetBool(false), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("OnlyPos"))) {
        getConfig().config.AddMember("OnlyPos", rapidjson::Value().SetBool(false), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("MoveWhilePlaying"))) {
        getConfig().config.AddMember("MoveWhilePlaying", rapidjson::Value().SetBool(true), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("MoveMultiplier"))) {
        getConfig().config.AddMember("MoveMultiplier", rapidjson::Value().SetFloat(5.0f), allocator);
    }

    if(getConfig().config.HasMember("Active") && !(getConfig().config.HasMember("DisableWhileReplay"))) {
        getConfig().config.AddMember("DisableWhileReplay", rapidjson::Value().SetBool(true), allocator);
    }

    if(getConfig().config.HasMember("Active")) {return;}

    // Add all the default options
    getConfig().config.RemoveAllMembers(); // Empty the config - it should already be empty but just to be sure
    getConfig().config.SetObject(); // Set the base of the config to a value that can contain keys

    // Get what is used to allocate memory in the config file
    // rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();

    // Add a member to the config, using the allocator

    getConfig().config.AddMember("Active", rapidjson::Value().SetBool(true), allocator);
    getConfig().config.AddMember("Fixed", rapidjson::Value().SetBool(false), allocator);
    getConfig().config.AddMember("LeftSaber", rapidjson::Value().SetBool(false), allocator);
    getConfig().config.AddMember("SwapSaber", rapidjson::Value().SetBool(false), allocator);
    getConfig().config.AddMember("ApplyOffsets", rapidjson::Value().SetBool(false), allocator);
    getConfig().config.AddMember("OnlyPos", rapidjson::Value().SetBool(false), allocator);
    getConfig().config.AddMember("MoveWhilePlaying", rapidjson::Value().SetBool(true), allocator);
    getConfig().config.AddMember("DisableWhileReplay", rapidjson::Value().SetBool(true), allocator);
    getConfig().config.AddMember("XOffset", rapidjson::Value().SetFloat(2.0), allocator);
    getConfig().config.AddMember("YOffset", rapidjson::Value().SetFloat(1.0), allocator);
    getConfig().config.AddMember("ZOffset", rapidjson::Value().SetFloat(-2.0), allocator);
    getConfig().config.AddMember("XRot", rapidjson::Value().SetFloat(0), allocator);
    getConfig().config.AddMember("YRot", rapidjson::Value().SetFloat(0), allocator);
    getConfig().config.AddMember("ZRot", rapidjson::Value().SetFloat(0), allocator);
    getConfig().config.AddMember("MoveMultiplier", rapidjson::Value().SetFloat(5.0f), allocator);

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
    //INSTALL_HOOK_OFFSETLESS(logger, VRController_Update, il2cpp_utils::FindMethodUnsafe("", "VRController", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(logger, SceneManager_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
    INSTALL_HOOK_OFFSETLESS(logger, AudioTimeSyncController_Start, il2cpp_utils::FindMethodUnsafe("", "AudioTimeSyncController", "Start", 0));

    getLogger().info("Installed all hooks!");
}
