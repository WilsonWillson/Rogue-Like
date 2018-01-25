#include "Engine/File/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Win32Includes.hpp"

bool LoadBinaryFileToBuffer(const std::string& filePath, std::vector< unsigned char >& out_buffer)
{
	FILE* file = nullptr;
	errno_t errorCode = fopen_s(&file, filePath.c_str(), "rb"); // open for reading in binary mode
	if (errorCode)
		return false;

	// Determine file size: "Seek" to to end of file, "tell" where we're at, then back to start
	fseek(file, 0, SEEK_END);
	size_t numBytesToRead = ftell(file);
	rewind(file);

	out_buffer.resize(numBytesToRead);
	if (numBytesToRead > 0)
		fread(&out_buffer[0], sizeof(unsigned char), numBytesToRead, file);

	fclose(file);
	return true;
}

bool SaveBufferToBinaryFile(const std::string& filePath, const std::vector< unsigned char >& buffer)
{
	FILE* file = nullptr;
	errno_t errorCode = fopen_s(&file, filePath.c_str(), "wb"); // open for writing in binary mode
	if (errorCode)
		return false;

	size_t numBytesToWrite = (int)buffer.size();
	if (numBytesToWrite > 0)
		fwrite(&buffer[0], sizeof(unsigned char), numBytesToWrite, file);

	fclose(file);
	return true;
}

bool SaveBufferToBinaryFile(const std::string& filePath, const std::vector< std::string >& buffer)
{
	FILE* file = nullptr;
	errno_t errorCode = fopen_s(&file, filePath.c_str(), "wb"); // open for writing in binary mode
	if (errorCode)
		return false;

	for (unsigned int bufferStep = 0; bufferStep < buffer.size(); bufferStep++)
	{
		unsigned int N(buffer[bufferStep].size());
		//fwrite(&N, sizeof(N), 1, file);
		fwrite(buffer[bufferStep].c_str(), 1, N, file);
		fwrite("\r\n", sizeof(char), 1, file);
		fflush(file);
	}
	fclose(file);
	return true;
}


//Code received from Stack overflow test on fastest way to find if file exsist
bool DoesFileExsist(const std::string& filePath)
{
	struct stat buffer;
	return (stat(filePath.c_str(), &buffer) == 0);
}


void* FileReadToBuffer(char const *filename, size_t *out_size)
{
	*out_size = 0U;

	// I'm going to use Windows file functions for no particular reason
	// outside of familiarity.  Feel free to use whatever. 
	DWORD file_access = GENERIC_READ;
	DWORD share = FILE_SHARE_READ;
	DWORD options = OPEN_EXISTING;

	HANDLE file_handle = CreateFileA(filename, file_access, share, NULL, options, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		DebuggerPrintf("File [%s] could not be opened.", filename);
		return nullptr;
	}

	// Get File Size
	DWORD high_word;
	DWORD lo_word = ::GetFileSize(file_handle, &high_word);

	// 64-bit way - use a 64-bit type, but we will never have a 4GB+ file, so ignore it and just using lo-word to prevent warnings
	// size_t size = ((size_t)high_word << 32) | (size_t)lo_word;
	size_t size = (size_t)lo_word;

	// Make a buffer that big
	void *buffer = malloc(size + 1U); // I allocae one extra for a null terminator - but do not include it in the size
									  // just in the cases I load strings it null terminates.
	if (nullptr != buffer) {
		DWORD read = 0U;
		ReadFile(file_handle, buffer, (DWORD)size, &read, nullptr);
		*out_size = read;

		((char*)buffer)[size] = NULL;
	}

	CloseHandle((HANDLE)file_handle);
	return buffer;
}
