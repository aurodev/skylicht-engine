#pragma once 

#include "easing.h"
#include <functional>

#define MAX_TWEEN_VALUE 16

namespace Skylicht
{
	class CTween
	{
	protected:
		float m_time;
		float m_delay;
		float m_duration;

		float m_percentTime;
		float m_percentValue;

		EEasingFunctions m_ease;
		EasingFunction m_function;

		float m_fromValue[MAX_TWEEN_VALUE];
		float m_toValue[MAX_TWEEN_VALUE];
		float m_value[MAX_TWEEN_VALUE];

		int m_numValue;

		bool m_start;
	public:
		std::function<void(CTween*)> OnUpdate;
		std::function<void(CTween*)> OnFinish;
		std::function<void(CTween*)> OnStart;
		std::function<void(CTween*)> OnStop;		
		std::function<void(CTween*)> OnDelay;

	public:
		CTween();

		virtual ~CTween();

		void update();

		void stop();

		virtual void updateValue() = 0;

	protected:
		void setBeginValue(int index, float value);

		void setEndValue(int index, float value);

		inline void setNumValue(int num)
		{
			m_numValue = num;
		}

		float getValueByIndex(int index)
		{
			return m_value[index];
		}

	public:
		inline void setEase(EEasingFunctions ease)
		{
			m_ease = ease;
			m_function = getEasingFunction(ease);
		}

		inline EEasingFunctions getEase()
		{
			return m_ease;
		}

		inline void setDelay(float delay)
		{
			m_delay = delay;
		}

		inline float getDelay()
		{
			return m_delay;
		}

		inline void setTime(float time)
		{
			m_time = time;
		}

		inline float getTime()
		{
			return m_time;
		}

		inline void setDuration(float duration)
		{
			m_duration = duration;
		}

		inline float getDuration()
		{
			return m_duration;
		}

		inline float getPercentTime()
		{
			return m_percentTime;
		}

		inline float getPercentValue()
		{
			return m_percentValue;
		}
	};
}