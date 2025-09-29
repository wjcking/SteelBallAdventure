#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H
#include <unordered_map>
class Clock
{
private:
	static float tik;
	static int iTick;

public:
	Clock() {};
	//ÿ�μ�0.005
	static float&  getTickFloat();
	static int&  getTickInteger();
	static float&  addDeltaCount();
};
//�ӳٶ���������
struct DelayPhase
{
	float delaySecond;
	float timestramp;
	DelayPhase(const float& d = 1.f)
	{
		delaySecond = d;
		timestramp = 0.f;
	};

	inline void reset() { timestramp = 0.f; }
	//�Ƿ�ʱ��
	inline bool isTimeUp()
	{
		if (timestramp == 0.f)
			timestramp = Clock::getTickFloat() + delaySecond;

		return Clock::getTickFloat() > timestramp;
	}

};
class DelayCollection
{
private:
	static std::unordered_map<short,DelayPhase> delay;
public:
	static void  clear();
	static void  reset(const short&);
	static bool isTimeUp(const short&,const float&);
};


#endif