#include "main.hpp"
#include "ModConfig.hpp"
#include "GlobalNamespace/LightManager.hpp"
#include "GlobalNamespace/VRControllersInputManager.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "GlobalNamespace/AvatarPoseController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Matrix4x4.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"

#include "Play3rdPerViewController.hpp"

#include "questui/shared/QuestUI.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "custom-types/shared/register.hpp"

#include <math.h>

using namespace GlobalNamespace;

static ModInfo modInfo;
DEFINE_CONFIG(ModConfig);

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
UnityEngine::Vector3 saberRot = UnityEngine::Vector3(getModConfig().XRot.GetValue(), getModConfig().YRot.GetValue(), getModConfig().ZRot.GetValue());
UnityEngine::Vector3 saberPos = UnityEngine::Vector3(getModConfig().XOffset.GetValue(), getModConfig().YOffset.GetValue(), getModConfig().ZOffset.GetValue());

UnityEngine::Vector3 prevPos = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
UnityEngine::Vector3 prevRot = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
// UnityEngine::Vector3 prevPosOffset = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
// UnityEngine::Vector3 prevRotOffset = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);

bool replay = false;
float rotated = 0.0f;

MAKE_HOOK_MATCH(LightManager_OnWillRenderObject, &LightManager::OnWillRenderObject, void, LightManager* self) {
  // Do stuff when this function is called 
  LightManager_OnWillRenderObject(self); 
  //if(!(getConfig().config["Active"].GetBool()) || replay) return;
  if(!getModConfig().Active.GetValue()) return;
  UnityEngine::Camera* c = UnityEngine::Camera::get_main();
  UnityEngine::Vector3 rot = c->get_transform()->get_eulerAngles();
  UnityEngine::Vector3 pos = c->get_transform()->get_position();

  if(getModConfig().LeftSaber.GetValue() && getModConfig().SwapSaber.GetValue() && rightController != nullptr) {
        saberPos = rightController->get_position();
        saberRot = rightController->get_rotation().get_eulerAngles();
    } else if(getModConfig().LeftSaber.GetValue() && leftController != nullptr) {
        saberPos = leftController->get_position();
        saberRot = leftController->get_rotation().get_eulerAngles();
    }

    if(getModConfig().MoveWhilePlaying.GetValue()) {
        GlobalNamespace::OVRInput::Update();
        if(GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Four, OVRInput::Controller::Touch)) {
            UnityEngine::Vector3 posDifference = pos - prevPos;
            getModConfig().XOffset.SetValue(posDifference.x * getModConfig().MoveMultiplier.GetValue() + getModConfig().XOffset.GetValue());
            getModConfig().YOffset.SetValue(posDifference.y * getModConfig().MoveMultiplier.GetValue() + getModConfig().YOffset.GetValue());
            getModConfig().ZOffset.SetValue(posDifference.z * getModConfig().MoveMultiplier.GetValue() + getModConfig().ZOffset.GetValue());
        }
        prevPos = pos;
        prevRot = rot;
    }

    UnityEngine::Vector3 offsetRot = UnityEngine::Vector3(getModConfig().XRot.GetValue(), getModConfig().YRot.GetValue(), getModConfig().ZRot.GetValue());
    UnityEngine::Vector3 offsetPos = UnityEngine::Vector3(getModConfig().XOffset.GetValue(), getModConfig().YOffset.GetValue(), getModConfig().ZOffset.GetValue());
    
  typedef function_ptr_t<void, UnityEngine::Camera*, UnityEngine::Matrix4x4> type;
auto method = *reinterpret_cast<type>(il2cpp_functions::resolve_icall("UnityEngine.Camera::set_cullingMatrix_Injected"));

method(c, UnityEngine::Matrix4x4::Ortho(-99999, 99999, -99999, 99999, 0.001f, 99999) * TranslateMatrix(UnityEngine::Vector3::get_forward() * -99999 / 2) * c->get_worldToCameraMatrix());

  if(getModConfig().Fixed.GetValue()) {
    if(getModConfig().LeftSaber.GetValue()) { 
        pos.x = saberPos.x + (getModConfig().ApplyOffsets.GetValue() ? offsetPos.x : 0.0f);
        pos.y = saberPos.y + (getModConfig().ApplyOffsets.GetValue() ? offsetPos.y : 0.0f);
        pos.z = saberPos.z + (getModConfig().ApplyOffsets.GetValue() ? offsetPos.z : 0.0f);

        if(!getModConfig().OnlyPos.GetValue()) {
            rot.x = saberRot.x + (getModConfig().ApplyOffsets.GetValue() ? offsetRot.x : 0.0f);
            rot.y = saberRot.y + (getModConfig().ApplyOffsets.GetValue() ? offsetRot.y : 0.0f);
            rot.z = saberRot.z + (getModConfig().ApplyOffsets.GetValue() ? offsetRot.z : 0.0f);
        }
    } else {
        pos.x = offsetPos.x;
        pos.y = offsetPos.y;
        pos.z = offsetPos.z;

        if(!getModConfig().OnlyPos.GetValue()) {
            rot.x = offsetRot.z;
            rot.y = offsetRot.y;
            rot.z = offsetRot.z;
        }
    }
  } else {
      if(getModConfig().LeftSaber.GetValue()) {
        pos.x += saberPos.x + (getModConfig().ApplyOffsets.GetValue() ? offsetPos.x : 0.0f);
        pos.y += saberPos.y + (getModConfig().ApplyOffsets.GetValue() ? offsetPos.y : 0.0f);
        pos.z += saberPos.z + (getModConfig().ApplyOffsets.GetValue() ? offsetPos.z : 0.0f);

        if(!getModConfig().OnlyPos.GetValue()) {
            rot.x += saberRot.x + (getModConfig().ApplyOffsets.GetValue() ? offsetRot.x : 0.0f);
            rot.y += saberRot.y + (getModConfig().ApplyOffsets.GetValue() ? offsetRot.y : 0.0f);
            rot.z += saberRot.z + (getModConfig().ApplyOffsets.GetValue() ? offsetRot.z : 0.0f);
        }
      } else {
        pos.x += offsetPos.x;
        pos.y += offsetPos.y;
        pos.z += offsetPos.z;

        if(!getModConfig().OnlyPos.GetValue()) {
            rot.x += offsetRot.z;
            rot.y += offsetRot.y;
            rot.z += offsetRot.z;
        }
      }
  } 

  if(getModConfig().WashingMachine.GetValue()) {
      rotated = remainderf(getModConfig().WashingMachineSpeed.GetValue() + rotated, 360.0f);
      rot.z += rotated;
  }

  c->get_transform()->set_position(pos);
  c->get_transform()->set_eulerAngles(rot);
}

MAKE_HOOK_MATCH(AudioTimeSyncController_Start, &AudioTimeSyncController::Start, void, AudioTimeSyncController* self) {
    AudioTimeSyncController_Start(self);
    if(getModConfig().DisableWhileReplay.GetValue() && (strcmp(getenv("ViewingReplay"), "true") == 0)) replay = true;
}

MAKE_HOOK_MATCH(SceneManager_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene nextActiveScene) {
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
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    // Install our hooks
    LoggerContextObject logger = getLogger().WithContext("load");
    INSTALL_HOOK(logger, LightManager_OnWillRenderObject);
    //INSTALL_HOOK_OFFSETLESS(logger, VRController_Update, il2cpp_utils::FindMethodUnsafe("", "VRController", "Update", 0));
    INSTALL_HOOK(logger, SceneManager_ActiveSceneChanged);
    //INSTALL_HOOK_OFFSETLESS(logger, AudioTimeSyncController_Start, il2cpp_utils::FindMethodUnsafe("", "AudioTimeSyncController", "Start", 0));

    getLogger().info("Installed all hooks!");
}