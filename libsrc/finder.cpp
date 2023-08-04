#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <math.h>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector; using std::thread;

static bool isFound = false;
static string findPath = "";
std::mutex savePathmtx;
vector<string> subDirs{};

int findInSubDirs(string dir, string searchTo)
{
	try
	{
		for(const auto & file: std::filesystem::recursive_directory_iterator(dir))
		{
			if(!std::filesystem::is_directory(file))
			{
				if (file.path().string().find(searchTo) != string::npos) 
				{
					savePathmtx.lock();
					if(!isFound)
					{
						findPath = file.path();
						isFound = true;
					}
					savePathmtx.unlock();
					return 1;
				} 
			}
		}
	}
	catch(std::filesystem::filesystem_error& e){}
	return 0;
}

int initSubDirs(string searchTo)
{
	for(const auto & file: std::filesystem::directory_iterator("/"))
	{
		if(std::filesystem::is_directory(file))
		{
			subDirs.push_back(file.path());
		}
		else
		{
			if (file.path().string().find(searchTo) != string::npos) 
			{
				findPath = file.path();
				return 1;
			} 
		}
	}
	return 0;
}

std::string findFile(string searchTo)
{
	if(initSubDirs(searchTo))
	{
		return findPath;
	}
	vector<thread> threads;
	threads.resize(7);
	for(size_t i = 0; i < ceil(subDirs.size() / 8.0) && !isFound; i++)
	{
		size_t j = 0;
		for(; i * 8 + j < subDirs.size() && j < 7 && !isFound; j++)
		{		
			threads[j] = thread(findInSubDirs, subDirs[i * 8 + j], searchTo);
		}
		if(i * 8 + j < subDirs.size() && !isFound)
			findInSubDirs(subDirs[j], searchTo);
		for(size_t k = 0; k < threads.size(); k++)
			threads[k].join();
	}
	return findPath;
}
