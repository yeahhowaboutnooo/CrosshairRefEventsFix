#include <SkyrimScripting/Plugin.h>

namespace CrosshairRefEventsFix {
    void fail() {
        logger::error("failed to fix disablePlayerControls overloading the script engine!");
        return;
    }

    void install() {
        auto fPlayerCharacter__PickCrosshairReference = REL::RelocationID(39534, 40620).address();
        if (!fPlayerCharacter__PickCrosshairReference) {
            fail();
            return;
        }
        uintptr_t offset = 0;

        switch (REL::Module::GetRuntime()) {
            case REL::Module::Runtime::SE:
                offset = 96;
                break;
            case REL::Module::Runtime::AE:
                offset = 101;
                break;
            default:
                fail();
                return;
        }

        auto injectionPoint = fPlayerCharacter__PickCrosshairReference + offset;

        std::vector<std::uint8_t> nops(6);
        std::fill(nops.begin(), nops.end(), (std::uint8_t)0x90);
        REL::safe_write<std::uint8_t>(injectionPoint, nops);

        logger::info("injected Code @ {0:x}", injectionPoint);
        logger::info("fixed disablePlayerControls overloading the script engine!");
    }
}

OnInit {
    CrosshairRefEventsFix::install();
}
