#pragma once
#include <chrono>
#include <typeinfo>

namespace MyTimer
{
	enum class TimeType
	{
		hour
		, minute
		, second
		, milisecond
		, nanosecond
	};
	using namespace std;
	using namespace chrono;
	class Timer
	{
		steady_clock::time_point tp;
		steady_clock::time_point tpElapsed;
		steady_clock::duration dur;
	public:
		Timer() : tp(steady_clock::now()), tpElapsed(steady_clock::now()), dur(milliseconds(10)) {}
		Timer(long long timeToWait, TimeType tt = TimeType::milisecond) 
			: tp(steady_clock::now())
			, dur(milliseconds(10))
			, tpElapsed(steady_clock::now())
		{
			SetDuration(timeToWait, tt);
		}

		void SetDuration(long long timeToWait, TimeType tt = TimeType::milisecond)
		{
			switch (tt)
			{
			case TimeType::hour: dur = hours(timeToWait);  break;
			case TimeType::minute: dur = minutes(timeToWait);  break;
			case TimeType::second: dur = seconds(timeToWait);  break;
			case TimeType::milisecond: dur = milliseconds(timeToWait);  break;
			case TimeType::nanosecond: dur = nanoseconds(timeToWait);  break;
			}
		}

		// true = time passed
		bool Check(bool reset = true)
		{
			if (tp <= steady_clock::now())
			{
				if (reset)
				{
					tp = steady_clock::now() + dur;
				}
				return true;
			}
			else return false;
		}

		// return in seconds
		float Elapsed(TimeType type = TimeType::milisecond)
		{
			float report = 0.0f;
			switch (type)
			{
			case TimeType::hour: 
			{
				auto val = duration_cast<hours>(steady_clock::now() - tpElapsed);
				report =  static_cast<float>(val.count()) * 60.0f * 24.0f;
				break;
			}
			case TimeType::minute:
			{
				auto val = duration_cast<minutes>(steady_clock::now() - tpElapsed);
				report = static_cast<float>(val.count()) * 60.0f;
				break;
			}
			case TimeType::second:
			{
				auto val = duration_cast<seconds>(steady_clock::now() - tpElapsed);
				report = static_cast<float>(val.count());
				break;
			}
			case TimeType::milisecond:
			{
				auto val = duration_cast<milliseconds>(steady_clock::now() - tpElapsed);
				report = static_cast<float>(val.count()) / 1000.0f;
				break;
			}
			case TimeType::nanosecond:
			{
				auto val = duration_cast<nanoseconds>(steady_clock::now() - tpElapsed);
				report = static_cast<float>(val.count()) / 1000000.0f; // NIE PRESNE !!!!
				break;
			}
			}
			tpElapsed = steady_clock::now();
			return report;
		}
	};
}