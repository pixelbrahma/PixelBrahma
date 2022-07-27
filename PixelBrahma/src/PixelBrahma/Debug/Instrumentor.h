#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace PixelBrahma
{
	// Structure containing profiling results information
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	// Instrumentation session information
	struct InstrumentationSession
	{
		std::string Name;
	};

	// Intrumentor class to handle profiling and writing results to json files
	class Instrumentor
	{
	public:
		Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

		// Begin profiling session
		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		// End profiling session
		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		// Write profiling information onto the output stream
		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		// Write header data of the file
		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		// Write footer data
		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Instrumentor getter function
		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	};

	// Instrumentation timer class for profiling and performance measurement
	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
		{
			// Timer start time
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		// Timer stop function
		void Stop()
		{
			// Timer end time
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};

// Macros for profiling
#define PB_PROFILE 1
#if PB_PROFILE
	#define PB_PROFILE_BEGIN_SESSION(name, filepath)   ::PixelBrahma::Instrumentor::Get().BeginSession(name, filepath)
	#define PB_PROFILE_END_SESSION()                   ::PixelBrahma::Instrumentor::Get().EndSession()
	#define PB_PROFILE_SCOPE(name)                     ::PixelBrahma::InstrumentationTimer timer##__LINE__(name);
	#define PB_PROFILE_FUNCTION() PB_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PB_PROFILE_BEGIN_SESSION(name, filepath)
	#define PB_PROFILE_END_SESSION()
	#define PB_PROFILE_SCOPE(name)
	#define PB_PROFILE_FUNCTION()
#endif
}