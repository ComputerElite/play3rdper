#include "main.hpp"

#include "custom-types/shared/macros.hpp"

#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ViewController.hpp"

// ViewController for the settings UI
DECLARE_CLASS_CODEGEN(Play3rdPer, Play3rdPerViewController, HMUI::ViewController,

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    //DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);

    REGISTER_FUNCTION(Play3rdPerViewController,
        //REGISTER_FIELD(modeSelectButton);
        //REGISTER_FIELD(thresholdSetting);

        REGISTER_METHOD(DidActivate);
        //REGISTER_METHOD(DidDeactivate);
    )
)