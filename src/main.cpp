#include "main.hpp"
#include "ModConfig.hpp"
#include "GlobalNamespace/LightManager.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/OVRInput.hpp"
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

#include "bsml/shared/BSML.hpp"

#include <math.h>

using namespace GlobalNamespace;

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

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
UnityEngine::Vector3 saberRot;
UnityEngine::Vector3 saberPos;

UnityEngine::Vector3 prevPos = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);

bool replay = false;
bool shouldGetControllers = true;
float rotated = 0.0f;
int framesPressed = 0;
int frame = 0;

MAKE_HOOK_MATCH(LightManager_OnWillRenderObject, &LightManager::OnCameraPreRender, void, LightManager* self, UnityEngine::Camera* camera) {
  // Do stuff when this function is called 
  LightManager_OnWillRenderObject(self, camera); 
  frame++;
  if(!getModConfig().Active.GetValue() || frame < 20) return;
  frame = 20;

if(shouldGetControllers) {
    ArrayW controllers = UnityEngine::Resources::FindObjectsOfTypeAll<VRController*>();
    for (int i = 0; i < controllers.size(); i++) {
        if(controllers[i]->get_node() == UnityEngine::XR::XRNode::LeftHand) {
            leftController = controllers[i];
        } else if(controllers[i]->get_node() == UnityEngine::XR::XRNode::RightHand){
            rightController = controllers[i];
        }
    }
    shouldGetControllers = false;
}

  UnityEngine::Camera* c = UnityEngine::Camera::get_main();
  if(!c) return;
  UnityEngine::Vector3 rot = c->get_transform()->get_eulerAngles();
  UnityEngine::Vector3 pos = c->get_transform()->get_position();


  if(getModConfig().LeftSaber.GetValue() && getModConfig().SwapSaber.GetValue() && rightController != nullptr) {
        saberPos = rightController->get_position();
        saberRot = rightController->get_rotation().get_eulerAngles();
    } else if(getModConfig().LeftSaber.GetValue() && leftController != nullptr) {
        saberPos = leftController->get_position();
        saberRot = leftController->get_rotation().get_eulerAngles();
    }

    if(GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::One, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Two, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Three, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Four, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::PrimaryHandTrigger, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::PrimaryIndexTrigger, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::SecondaryHandTrigger, OVRInput::Controller::Touch) && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::SecondaryIndexTrigger, OVRInput::Controller::Touch)) {
        framesPressed++;
        if(framesPressed >= 1000) {
            getModConfig().XOffset.SetValue(0.0f);
            getModConfig().YOffset.SetValue(0.0f);
            getModConfig().ZOffset.SetValue(0.0f);
            getModConfig().XRot.SetValue(0.0f);
            getModConfig().YRot.SetValue(0.0f);
            getModConfig().ZRot.SetValue(0.0f);
            getModConfig().WashingMachine.SetValue(false);
            getModConfig().Active.SetValue(false);
            framesPressed = 0;
        }
    }
    else if(getModConfig().MoveWhilePlaying.GetValue()) {
        UnityEngine::Vector3 diffPos = getModConfig().MoveController.GetValue() == 0 ? c->get_transform()->get_position() : (getModConfig().MoveController.GetValue() == 1 ? leftController->get_position() : rightController->get_position());
        GlobalNamespace::OVRInput::Update();
        if(GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Four, OVRInput::Controller::Touch)) {
            UnityEngine::Vector3 posDifference = UnityEngine::Vector3::op_Subtraction(diffPos, prevPos);

            getModConfig().XOffset.SetValue(posDifference.x * getModConfig().MoveMultiplier.GetValue() + getModConfig().XOffset.GetValue());
            getModConfig().YOffset.SetValue(posDifference.y * getModConfig().MoveMultiplier.GetValue() + getModConfig().YOffset.GetValue());
            getModConfig().ZOffset.SetValue(posDifference.z * getModConfig().MoveMultiplier.GetValue() + getModConfig().ZOffset.GetValue());
        }
        prevPos = diffPos;
    }
    else framesPressed = 0;

    UnityEngine::Vector3 offsetRot = UnityEngine::Vector3(getModConfig().XRot.GetValue(), getModConfig().YRot.GetValue(), getModConfig().ZRot.GetValue());
    UnityEngine::Vector3 offsetPos = UnityEngine::Vector3(getModConfig().XOffset.GetValue(), getModConfig().YOffset.GetValue(), getModConfig().ZOffset.GetValue());
    
  typedef function_ptr_t<void, UnityEngine::Camera*, UnityEngine::Matrix4x4> type;
auto method = *reinterpret_cast<type>(il2cpp_functions::resolve_icall("UnityEngine.Camera::set_cullingMatrix_Injected"));

auto vector = UnityEngine::Vector3::op_Multiply(UnityEngine::Vector3::get_forward(), -99999 / 2);
auto matrix = UnityEngine::Matrix4x4::op_Multiply(UnityEngine::Matrix4x4::Ortho(-99999, 99999, -99999, 99999, 0.001f, 99999), TranslateMatrix(vector));
matrix = UnityEngine::Matrix4x4::op_Multiply(matrix, c->get_worldToCameraMatrix());

method(c, matrix);

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
    shouldGetControllers = true;
    frame = 0;
    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
}

PLAY_3RD_PERSON_EXPORT void setup(CModInfo* info) {
    *info = modInfo.to_c();

    Paper::Logger::RegisterFileContextId(Logger.tag);
	
    getModConfig().Init(modInfo);
    Logger.info("Completed setup!");
}

PLAY_3RD_PERSON_EXPORT void late_load() {
    il2cpp_functions::Init();

    Logger.info("Installing hooks...");
    
    BSML::Init();
    BSML::Register::RegisterMainMenu("Play3rdPerson", "Play3rdPerson", "Play3rdPerson mod settings", DidActivate);
    BSML::Register::RegisterSettingsMenu("Play3rdPerson", DidActivate, false);
    // Install our hooks
    INSTALL_HOOK(Logger, LightManager_OnWillRenderObject);
    INSTALL_HOOK(Logger, SceneManager_ActiveSceneChanged);

    Logger.info("Installed all hooks!");
}