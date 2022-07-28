#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <iomanip>

namespace PixelBrahma
{
	using FloatingPointMicroSeconds = std::chrono::duration<double, std::micro>;

	// Structure containing profiling results information
	struct ProfileResult
	{
		std::string Name;
		FloatingPointMicroSeconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
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
		Instrumentor() : m_CurrentSession(nullptr) {}

		// Begin profiling session
		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			// Mutex lock against profiling multiple threads
			std::lock_guard lock(m_Mutex);

			if (m_CurrentSession) 
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) 
				{ 
					// Edge case: BeginSession() might be before Log::Init()
					PB_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", 
						name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}

			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open()) 
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else 
			{
				if (Log::GetCoreLogger()) 
				{ 
					// Edge case: BeginSession() might be before Log::Init()
					PB_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		// End profiling session
		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		// Write profiling information onto the output stream
		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			
			if (m_CurrentSession) 
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		// Instrumentor getter function
		static Instrumentor& Get() 
		{
			static Instrumentor instance;
			return instance;
		}

		// Write header data of the file
		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		// Write footer data
		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession() 
		{
			if (m_CurrentSession) 
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	// Instrumentation timer class for profiling and performance measurement
	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
		{
			// Timer start time
			m_StartTimepoint = std::chrono::steady_clock::now();
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
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroSeconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).
				time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).
				time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

// Macros for profiling
#define PB_PROFILE 0
#if PB_PROFILE
	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define PB_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define PB_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
		#define PB_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define PB_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define PB_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define PB_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define PB_FUNC_SIG __func__
	#else
		#define PB_FUNC_SIG "PB_FUNC_SIG unknown!"
	#endif

	#define PB_PROFILE_BEGIN_SESSION(name, filepath)   ::PixelBrahma::Instrumentor::Get().BeginSession(name, filepath)
	#define PB_PROFILE_END_SESSION()                   ::PixelBrahma::Instrumentor::Get().EndSession()
	#define PB_PROFILE_SCOPE(name)                     ::PixelBrahma::InstrumentationTimer timer##__LINE__(name);
	#define PB_PROFILE_FUNCTION() PB_PROFILE_SCOPE(PB_FUNC_SIG)
#else
	#define PB_PROFILE_BEGIN_SESSION(name, filepath)
	#define PB_PROFILE_END_SESSION()
	#define PB_PROFILE_SCOPE(name)
	#define PB_PROFILE_FUNCTION()
#endif
}