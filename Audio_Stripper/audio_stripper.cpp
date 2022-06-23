#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct Riff
{
	unsigned char	chunkID[4] = { 0 , 0 , 0 , 0 };
	unsigned int	chunkSize;
	unsigned char	format[4] = { 0 , 0 , 0 , 0 };
	unsigned char	subChunkOneID[4] = { 0 , 0 , 0 , 0 };
	unsigned int	subChunkOneSize;
	unsigned short	audioFormat;
	unsigned short	numChannels;
	unsigned int	sampleRate;
	unsigned int	byteRate;
	unsigned short	blockAlign;
	unsigned short	bitPerSample;
	unsigned short	extraParamSize;
	std::vector<unsigned char> extraParam;
	unsigned char	subChunkTwoID[4] = { 0 , 0 , 0 , 0 };
	unsigned int	subChunkTwoSize;
	std::vector<unsigned char> audio;

	Riff()
	{
		chunkSize = 0;
		subChunkOneSize = 0;
		audioFormat = 0;
		numChannels = 0;
		sampleRate = 0;
		byteRate = 0;
		blockAlign = 0;
		bitPerSample = 0;
		extraParamSize = 0;
		subChunkTwoSize = 0;
	}
};

void Initialize(std::string& userInputPath, size_t& count)
{
	std::cout << "Welcome to Audio Stripper!" << std::endl;
	std::cout << std::endl;
	std::cout << "This software was made to strip the spurious data that is " << std::endl;
	std::cout << "typically stored at the end of a WAVE file." << std::endl;
	std::cout << std::endl;
	std::cout << "The path of the file must be written such as the example below." << std::endl;
	std::cout << "Example: C:\\\\Paste\\\\file.wav" << std::endl;
	std::cout << std::endl;
	std::cout << "Please, type the path of the file that you want to strip away the metadata:" << std::endl;
	std::cin >> userInputPath;
	std::cout << std::endl;
	count = userInputPath.size();
}

bool RiffChecker(Riff* input)
{
	if (input->chunkID[0] != 'R' && input->chunkID[1] != 'I' && input->chunkID[2] != 'F' && input->chunkID[3] != 'F')
		return false;
	else
		return true;
}

void ReadsHeader(Riff* emptyFileChunks, std::ifstream& inputFile)
{
	unsigned char extraParamByte;

	inputFile.read((char*)&emptyFileChunks->chunkID, 4);
	inputFile.read((char*)&emptyFileChunks->chunkSize, 4);
	inputFile.read((char*)&emptyFileChunks->format, 4);

	inputFile.read((char*)&emptyFileChunks->subChunkOneID, 4);
	inputFile.read((char*)&emptyFileChunks->subChunkOneSize, 4);
	inputFile.read((char*)&emptyFileChunks->audioFormat, 2);
	inputFile.read((char*)&emptyFileChunks->numChannels, 2);
	inputFile.read((char*)&emptyFileChunks->sampleRate, 4);
	inputFile.read((char*)&emptyFileChunks->byteRate, 4);
	inputFile.read((char*)&emptyFileChunks->blockAlign, 2);
	inputFile.read((char*)&emptyFileChunks->bitPerSample, 2);

	if (emptyFileChunks->subChunkOneSize > 16)
	{
		inputFile.read((char*)&emptyFileChunks->extraParamSize, 2);

		for (int i = 0; i < emptyFileChunks->extraParamSize; i++)
		{
			inputFile.read((char*)&extraParamByte, 1);
			emptyFileChunks->extraParam.push_back(extraParamByte);
		}
	}

	inputFile.read((char*)&emptyFileChunks->subChunkTwoID, 4);
	inputFile.read((char*)&emptyFileChunks->subChunkTwoSize, 4);
}

void ReadsAudio(Riff* emptyFileChunks, std::ifstream& inputFile)
{
	unsigned char audioByte;

	for (unsigned int j = 0; j < emptyFileChunks->subChunkTwoSize; j++)
	{
		inputFile.read((char*)&audioByte, 1);
		emptyFileChunks->audio.push_back(audioByte);
	}
}

void PrintHeader(const Riff* parsedHeader)
{
	std::cout << "Header Information" << std::endl;
	std::cout << std::endl;
	std::cout << "Chunk ID: ";
	std::cout << parsedHeader->chunkID[0];
	std::cout << parsedHeader->chunkID[1];
	std::cout << parsedHeader->chunkID[2];
	std::cout << parsedHeader->chunkID[3] << std::endl;
	std::cout << "Chunk Size: ";
	std::cout << parsedHeader->chunkSize << " bytes" << std::endl;
	std::cout << "Format: ";
	std::cout << parsedHeader->format[0];
	std::cout << parsedHeader->format[1];
	std::cout << parsedHeader->format[2];
	std::cout << parsedHeader->format[3] << std::endl;

	std::cout << "Subchunk 1 ID: ";
	std::cout << parsedHeader->subChunkOneID[0];
	std::cout << parsedHeader->subChunkOneID[1];
	std::cout << parsedHeader->subChunkOneID[2];
	std::cout << parsedHeader->subChunkOneID[3] << std::endl;
	std::cout << "Subchunk 1 Size: ";
	std::cout << parsedHeader->subChunkOneSize << " bytes" << std::endl;
	std::cout << "Audio Format: ";
	std::cout << parsedHeader->audioFormat << std::endl;
	std::cout << "Number of Channels: ";
	std::cout << parsedHeader->numChannels << std::endl;
	std::cout << "Sample Rate: ";
	std::cout << parsedHeader->sampleRate << " Hz" << std::endl;
	std::cout << "Byte Rate: ";
	std::cout << parsedHeader->byteRate << std::endl;
	std::cout << "Block Align: ";
	std::cout << parsedHeader->blockAlign << std::endl;
	std::cout << "Bits per Sample: ";
	std::cout << parsedHeader->bitPerSample << " bits" << std::endl;

	std::cout << "Subchunk 2 ID: ";
	std::cout << parsedHeader->subChunkTwoID[0];
	std::cout << parsedHeader->subChunkTwoID[1];
	std::cout << parsedHeader->subChunkTwoID[2];
	std::cout << parsedHeader->subChunkTwoID[3] << std::endl;
	std::cout << "Subchunk Two Size (audio): ";
	std::cout << parsedHeader->subChunkTwoSize << " bytes" << std::endl;
	std::cout << std::endl;
}

void CreatesNewFile(Riff* filledChunks)
{
	unsigned char extraParamByteWrite;
	unsigned char audioByteWrite;

	std::ofstream outputFile("New_Wave.wav", std::ios::binary);

	outputFile.write((char*)&filledChunks->chunkID, 4);
	outputFile.write((char*)&filledChunks->chunkSize, 4);
	outputFile.write((char*)&filledChunks->format, 4);

	outputFile.write((char*)&filledChunks->subChunkOneID, 4);
	outputFile.write((char*)&filledChunks->subChunkOneSize, 4);
	outputFile.write((char*)&filledChunks->audioFormat, 2);
	outputFile.write((char*)&filledChunks->numChannels, 2);
	outputFile.write((char*)&filledChunks->sampleRate, 4);
	outputFile.write((char*)&filledChunks->byteRate, 4);
	outputFile.write((char*)&filledChunks->blockAlign, 2);
	outputFile.write((char*)&filledChunks->bitPerSample, 2);

	if (filledChunks->subChunkOneSize > 16)
	{
		outputFile.write((char*)&filledChunks->extraParamSize, 2);

		for (int i = 0; i < filledChunks->extraParamSize; i++)
		{
			extraParamByteWrite = filledChunks->extraParam[i];
			outputFile.write((char*)&extraParamByteWrite, 1);
		}
	}

	outputFile.write((char*)&filledChunks->subChunkTwoID, 4);
	outputFile.write((char*)&filledChunks->subChunkTwoSize, 4);

	for (unsigned int j = 0; j < filledChunks->subChunkTwoSize; j++)
	{
		audioByteWrite = filledChunks->audio[j];
		outputFile.write((char*)&audioByteWrite, 1);
	}

	std::cout << "[LOG]:New .wav file created successfully!" << std::endl;
}

int main()
{
	bool checksRiff;
	const int MAX_LENGTH = 100;
	const int MIN_LENGTH = 4;
	std::string filePath;
	size_t filePathCount = 0;
	Riff* fileChunks = new Riff();

	Initialize(filePath, filePathCount);
	if (filePathCount < MIN_LENGTH)
	{
		std::cout << "[ERROR]:This path is too short!" << std::endl;
		return EXIT_FAILURE;
	}

	if (filePathCount > MAX_LENGTH)
	{
		std::cout << "[ERROR]:This path is too large!" << std::endl;
		return EXIT_FAILURE;
	}

	if (filePath[filePathCount - 4] != '.' && filePath[filePathCount - 3] != 'w' && filePath[filePathCount - 2] != 'a' && filePath[filePathCount - 2] != 'v')
	{
		std::cout << "[ERROR]:The file you are trying to parse is not a RIFF file or the path is invalid!" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream file(filePath, std::ios::binary);

	ReadsHeader(fileChunks, file);

	checksRiff = RiffChecker(fileChunks);
	if (!checksRiff)
	{
		std::cout << "[ERROR]:The file you are trying to parse is not a RIFF file or the path is invalid!" << std::endl;
		return EXIT_FAILURE;
	}

	ReadsAudio(fileChunks, file);
	PrintHeader(fileChunks);
	CreatesNewFile(fileChunks);
	std::cin.get();
}