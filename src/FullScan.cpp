#include "FullScan.h"

#include "Manager.h"
#include "SwapExporter.h"

#include <vector>

namespace BaseObjectSwapper::FullScan
{
    void RunDiagnostics()
    {
        REX::INFO("BOS Exporter: Starting full global swap export");

        RE::DebugNotification(
            "BOS Exporter: generating full swap log...",
            nullptr,
            false);

        const auto& [allForms, allFormsLock] =
            RE::TESForm::GetAllForms();

        if (!allForms) {
            REX::ERROR(
                "BOS Global Export: TESForm::GetAllForms returned null");

            RE::DebugMessageBox(
                "BOS Exporter failed.\n"
                "TESForm::GetAllForms returned null.\n"
                "Check the BOS log.");

            return;
        }

        std::vector<RE::TESObjectREFR*> refs;

        {
            [[maybe_unused]] const RE::BSReadLockGuard lockGuard{
                allFormsLock
            };

            for (auto it = allForms->begin();
                 it != allForms->end();
                 ++it) {

                auto* form = it->second;

                if (!form) {
                    continue;
                }

                if (auto* ref = form->As<RE::TESObjectREFR>()) {
                    refs.push_back(ref);
                }
            }
        }

        REX::INFO(
            "BOS Exporter: Collected {} REFRs from global form map",
            refs.size());

        auto* manager =
            FormSwap::Manager::GetSingleton();

        if (!manager) {
            REX::ERROR(
                "BOS Global Export: Manager singleton not available");

            RE::DebugMessageBox(
                "BOS Exporter failed.\n"
                "BOS Manager was not available.\n"
                "Check the BOS log.");

            return;
        }

        manager->LoadFormsOnce();

        std::size_t refsSeen = 0;
        std::size_t refsWithBase = 0;
        std::size_t refsWithUsableCell = 0;
        std::size_t swapsExported = 0;

        for (auto* ref : refs) {
            ++refsSeen;

            if (!ref) {
                continue;
            }

            auto* base = ref->GetBaseObject();

            if (!base) {
                continue;
            }

            ++refsWithBase;

            if (!(ref->GetParentCell() ||
                  ref->GetSaveParentCell())) {
                continue;
            }

            ++refsWithUsableCell;

            const auto swapData =
                manager->GetSwapData(ref, base);

            auto* swapBase = swapData.first;

            if (swapBase &&
                swapBase != base) {

                SwapExporter::RecordSwap(
                    ref,
                    swapBase);

                ++swapsExported;
            }
        }

        REX::INFO("{:*^30}", "BOS GLOBAL EXPORT");

        REX::INFO(
            "REFRs collected from map : {}",
            refs.size());

        REX::INFO(
            "REFRs processed          : {}",
            refsSeen);

        REX::INFO(
            "REFRs with base          : {}",
            refsWithBase);

        REX::INFO(
            "REFRs with usable cell   : {}",
            refsWithUsableCell);

        REX::INFO(
            "Swaps exported           : {}",
            swapsExported);

        REX::INFO(
            "{:*^30}",
            "END GLOBAL EXPORT");

        REX::INFO(
            "BOS Exporter: Finished. {} swaps exported",
            swapsExported);

        if (swapsExported > 0) {
            RE::DebugNotification(
                "BOS Exporter: full swap log finished.",
                nullptr,
                false);

            RE::DebugMessageBox(
                "BOS Exporter finished.\n"
                "BOS_Swaps.tsv is ready.\n"
                "You can close Skyrim.");
        } else {
            RE::DebugMessageBox(
                "BOS Exporter finished, but no swaps were found.\n"
                "Check the BOS log before closing Skyrim.");
        }
    }
}
