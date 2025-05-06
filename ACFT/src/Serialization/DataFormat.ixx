module;

#include <fstream>
#include <mutex>

export module ACFT.DataFormat;

import <filesystem>;
import <string>;
import <utility>;
import <variant>;
import <unordered_map>;

import Types;
import ACFT.Buffer;

namespace ACFT::DataFormat
{
	export const char* DataFileExt = "ado";

	export enum class FormatType
	{
		binary = 0, string
	};
	
	export enum class OpType
	{
		in = 0, out, in_and_out
	};
	
	//A file object that is associated with an actual binary obj file.
	export class ObjDataFile
	{
	public:
		static Ref<ObjDataFile> Open(const std::filesystem::path& file);

	public:
		ObjDataFile(const std::filesystem::path& file);
		~ObjDataFile();

		Ref<Buffer> ReadObj();

		void WriteObj(Ref<Buffer> data);

		bool IsReadable();

		void Clear();

	private:
		std::filesystem::path m_Path;
		std::fstream m_FStream;
		std::atomic<bool> m_Writing;
		std::mutex m_Mtx;
	};
}
