#include "SwapExporter.h"

#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace BaseObjectSwapper::SwapExporter
{
	namespace
	{
		std::mutex exportMutex;
		std::ofstream exportFile;
		bool exportInitialized = false;

		std::string GetStableFormID(const RE::TESForm* a_form)
		{
			if (!a_form || a_form->IsDynamicForm()) {
				return {};
			}

			const auto* file = a_form->GetFile(0);
			if (!file) {
				return {};
			}

			std::string_view fileName = file->GetFilename();

			// Match BOS's own handling of vanilla Skyrim references.
			if (a_form->AsReference() &&
			    (a_form->GetFormID() & 0xFF000000) == 0) {
				fileName = "Skyrim.esm";
			}

			std::ostringstream stream;
			stream << fileName
			       << '\t'
			       << std::uppercase
			       << std::hex
			       << std::setw(8)
			       << std::setfill('0')
			       << a_form->GetLocalFormID();

			return stream.str();
		}

		bool InitializeExporter()
		{
			if (exportInitialized) {
				return exportFile.is_open();
			}

			exportInitialized = true;

			const auto logDirectory = SKSE::log::log_directory();
			if (!logDirectory) {
				REX::ERROR("BOS Exporter: Could not find SKSE log directory");
				return false;
			}

			const auto exportPath = *logDirectory / "BOS_Swaps.tsv";

			exportFile.open(exportPath, std::ios::out | std::ios::trunc);

			if (!exportFile.is_open()) {
				REX::ERROR(
					"BOS Exporter: Could not open {}",
					exportPath.string());
				return false;
			}

			exportFile
				<< "reference_plugin"
				<< '\t'
				<< "reference_local_formid"
				<< '\t'
				<< "replacement_plugin"
				<< '\t'
				<< "replacement_local_formid"
				<< '\n';

			exportFile.flush();

			REX::INFO(
				"BOS Exporter: Writing swaps to {}",
				exportPath.string());

			return true;
		}
	}

	void RecordSwap(
		const RE::TESObjectREFR* a_ref,
		const RE::TESBoundObject* a_swapBase)
	{
		const auto reference = GetStableFormID(a_ref);
		const auto replacement = GetStableFormID(a_swapBase);

		if (reference.empty() || replacement.empty()) {
			return;
		}

		std::scoped_lock lock(exportMutex);

		if (!InitializeExporter()) {
			return;
		}

		exportFile
			<< reference
			<< '\t'
			<< replacement
			<< '\n';

		exportFile.flush();
	}
}
