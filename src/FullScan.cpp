#include "FullScan.h"

namespace BaseObjectSwapper::FullScan
{
    void RunDiagnostics()
    {
        const auto& [allForms, allFormsLock] =
            RE::TESForm::GetAllForms();

        if (!allForms) {
            REX::ERROR("BOS Global Scan: TESForm::GetAllForms returned null");
            return;
        }

        [[maybe_unused]] const RE::BSReadLockGuard lockGuard{
            allFormsLock
        };

        std::size_t totalForms = 0;
        std::size_t validForms = 0;

        std::size_t cellForms = 0;
        std::size_t referenceForms = 0;

        std::size_t refsWithBase = 0;
        std::size_t refsWithFile = 0;
        std::size_t refsWithParentCell = 0;
        std::size_t refsWithSaveParentCell = 0;
        std::size_t refsWithEitherCell = 0;

        std::size_t dynamicRefs = 0;

        for (auto it = allForms->begin();
             it != allForms->end();
             ++it) {

            ++totalForms;

            auto* form = it->second;

            if (!form) {
                continue;
            }

            ++validForms;

            if (form->As<RE::TESObjectCELL>()) {
                ++cellForms;
            }

            auto* ref = form->As<RE::TESObjectREFR>();

            if (!ref) {
                continue;
            }

            ++referenceForms;

            if (ref->IsDynamicForm()) {
                ++dynamicRefs;
            }

            if (ref->GetBaseObject()) {
                ++refsWithBase;
            }

            if (ref->GetFile(0)) {
                ++refsWithFile;
            }

            const bool hasParent =
                ref->GetParentCell() != nullptr;

            const bool hasSaveParent =
                ref->GetSaveParentCell() != nullptr;

            if (hasParent) {
                ++refsWithParentCell;
            }

            if (hasSaveParent) {
                ++refsWithSaveParentCell;
            }

            if (hasParent || hasSaveParent) {
                ++refsWithEitherCell;
            }
        }

        REX::INFO("{:*^30}", "BOS GLOBAL SCAN");
        REX::INFO(
            "All forms in map         : {}",
            totalForms);
        REX::INFO(
            "Valid TESForm entries    : {}",
            validForms);
        REX::INFO(
            "CELL forms               : {}",
            cellForms);
        REX::INFO(
            "REFR forms               : {}",
            referenceForms);
        REX::INFO(
            "Dynamic REFRs            : {}",
            dynamicRefs);
        REX::INFO(
            "REFRs with base          : {}",
            refsWithBase);
        REX::INFO(
            "REFRs with file          : {}",
            refsWithFile);
        REX::INFO(
            "REFRs with parent cell   : {}",
            refsWithParentCell);
        REX::INFO(
            "REFRs with save cell     : {}",
            refsWithSaveParentCell);
        REX::INFO(
            "REFRs with either cell   : {}",
            refsWithEitherCell);
        REX::INFO("{:*^30}", "END GLOBAL SCAN");
    }
}
