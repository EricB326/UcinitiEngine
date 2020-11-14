#ifndef TIME_STEP_H
#define TIME_STEP_H

namespace Uciniti
{
	class time_step
	{
	public: 
		time_step(float a_time = 0.0f)
			: time(a_time)
		{
		}

		operator float() const { return time; }

		float get_seconds() const { return time; }
		float get_milliseconds() const { return time * 1000.0f; }
	
	private:
		float time;
	};
}

#endif // !TIME_STEP_H