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

        auto& cells =
            dataHandler->GetFormArray(RE::FormType::Cell);

        std::size_t cellArrayEntries = 0;
        std::size_t validCells = 0;
        std::size_t attachedCells = 0;
        std::size_t unattachedCells = 0;
        std::size_t cellsWithRefs = 0;

        std::size_t totalRefs = 0;
        std::size_t refsInAttachedCells = 0;
        std::size_t refsInUnattachedCells = 0;
        std::size_t refsWithBase = 0;
        std::size_t refsWithFile = 0;

        for (auto* form : cells) {
            ++cellArrayEntries;

            if (!form) {
                continue;
            }

            auto* cell = form->As<RE::TESObjectCELL>();

            if (!cell) {
                continue;
            }

            ++validCells;

            const bool attached = cell->IsAttached();

            if (attached) {
                ++attachedCells;
            } else {
                ++unattachedCells;
            }

            std::size_t refsInThisCell = 0;

            cell->ForEachReference(
                [&](RE::TESObjectREFR* ref) {
                    if (!ref) {
                        return RE::BSContainer::ForEachResult::kContinue;
                    }

                    ++refsInThisCell;
                    ++totalRefs;

                    if (attached) {
                        ++refsInAttachedCells;
                    } else {
                        ++refsInUnattachedCells;
                    }

                    if (ref->GetBaseObject()) {
                        ++refsWithBase;
                    }

                    if (ref->GetFile(0)) {
                        ++refsWithFile;
                    }

                    return RE::BSContainer::ForEachResult::kContinue;
                });

            if (refsInThisCell > 0) {
                ++cellsWithRefs;
            }
        }

        REX::INFO("{:*^30}", "BOS CELL SCAN");
        REX::INFO(
            "Cell array entries       : {}",
            cellArrayEntries);
        REX::INFO(
            "Valid TESObjectCELL      : {}",
            validCells);
        REX::INFO(
            "Attached cells           : {}",
            attachedCells);
        REX::INFO(
            "Unattached cells         : {}",
            unattachedCells);
        REX::INFO(
            "Cells containing refs    : {}",
            cellsWithRefs);
        REX::INFO(
            "Total cell references    : {}",
            totalRefs);
        REX::INFO(
            "Refs in attached cells   : {}",
            refsInAttachedCells);
        REX::INFO(
            "Refs in unattached cells : {}",
            refsInUnattachedCells);
        REX::INFO(
            "References with base     : {}",
            refsWithBase);
        REX::INFO(
            "References with file     : {}",
            refsWithFile);
        REX::INFO("{:*^30}", "END CELL SCAN");
    }
}
