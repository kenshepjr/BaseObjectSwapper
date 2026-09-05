#include "FullScan.h"

namespace BaseObjectSwapper::FullScan
{
    void RunDiagnostics()
    {
        auto* dataHandler = RE::TESDataHandler::GetSingleton();

        if (!dataHandler) {
            REX::ERROR("BOS Full Scan: TESDataHandler not available");
            return;
        }

        auto& forms =
            dataHandler->GetFormArray(RE::FormType::Reference);

        std::size_t arrayEntries = 0;
        std::size_t validRefs = 0;
        std::size_t dynamicRefs = 0;
        std::size_t refsWithFile = 0;
        std::size_t refsWithBase = 0;
        std::size_t refsWithParentCell = 0;
        std::size_t refsWithSaveParentCell = 0;
        std::size_t refsWithEitherCell = 0;

        for (auto* form : forms) {
            ++arrayEntries;

            if (!form) {
                continue;
            }

            const auto* ref = form->As<RE::TESObjectREFR>();

            if (!ref) {
                continue;
            }

            ++validRefs;

            if (ref->IsDynamicForm()) {
                ++dynamicRefs;
            }

            if (ref->GetFile(0)) {
                ++refsWithFile;
            }

            if (ref->GetBaseObject()) {
                ++refsWithBase;
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

        REX::INFO("{:*^30}", "BOS FULL SCAN");
        REX::INFO(
            "Reference array entries : {}",
            arrayEntries);
        REX::INFO(
            "Valid TESObjectREFR     : {}",
            validRefs);
        REX::INFO(
            "Dynamic references      : {}",
            dynamicRefs);
        REX::INFO(
            "References with file    : {}",
            refsWithFile);
        REX::INFO(
            "References with base    : {}",
            refsWithBase);
        REX::INFO(
            "References with parent  : {}",
            refsWithParentCell);
        REX::INFO(
            "References with save cell: {}",
            refsWithSaveParentCell);
        REX::INFO(
            "References with either  : {}",
            refsWithEitherCell);
        REX::INFO("{:*^30}", "END FULL SCAN");
    }
}
