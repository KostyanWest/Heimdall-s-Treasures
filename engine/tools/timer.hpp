#ifndef HEIMDALL_ENGINE_TOOLS_TIMER_HPP
#define HEIMDALL_ENGINE_TOOLS_TIMER_HPP

#include <chrono>



namespace heimdall
{
namespace engine
{
namespace tools
{



/*
Хранит минимально и максимално возможные значения 
кратности интервалов таймеров в миллисекундах.
*/
struct timecaps
{
	unsigned int min_period;
	unsigned int max_period;
};



/*
Удобный класс для отсчёта одинаковых интервалов времени. 
Способен изменять кратноть интервалов таймеров для всего приложения.
*/
class timer
{
private:
	using clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::milliseconds;

public:
	/*
	Создаёт таймер с настройками по умолчанию.
	*/
	timer() noexcept {}

	/*
	Создаёт таймер с заданными настройками.

	@param interval - длительность интервалов в миллисекундах, которые будет отсчитывать таймер.
	@param time_point - начальная точка отсчёта.
	*/
	explicit timer( ms::rep interval, clock::time_point time_point = clock::now() ) noexcept
		: interval( interval ), time_point( time_point )
	{
	}

	/*
	Создаёт таймер с заданными настройками.

	@param time_point - начальная точка отсчёта.
	@param interval - длительность интервалов в миллисекундах, которые будет отсчитывать таймер.
	*/
	explicit timer( clock::time_point time_point, ms::rep interval = 15 ) noexcept
		: interval( interval ), time_point( time_point )
	{
	}

	/*
	Создаёт копию таймера с такими же интервалом и точкой отсчёта, 
	а также повторно устанавливает кратность интервалов таймеров для всего приложения.
	*/
	timer( const timer& other ) noexcept
		: interval( other.interval ), time_point( other.time_point )
	{
		set_tick_interval( other.tick_interval );
	}

	/*
	Создаёт копию таймера с такими же интервалом и точкой отсчёта,
	но право на владение кратности интервалов таймеров для всего приложения 
	передаётся новому экземпляру, старый экземпляр лишается этого права.
	*/
	timer( timer&& other ) noexcept
		: interval( other.interval ), time_point( other.time_point ), tick_interval( other.tick_interval )
	{
		other.reset_tick_interval();
	}

	/*
	Создаёт копию таймера с такими же интервалом и точкой отсчёта,
	а также повторно устанавливает кратность интервалов таймеров для всего приложения.
	*/
	timer& operator = ( const timer& other ) noexcept
	{
		interval = other.interval;
		time_point = other.time_point;
		set_tick_interval( other.tick_interval );
		return *this;
	}

	/*
	Создаёт копию таймера с такими же интервалом и точкой отсчёта,
	но право на владение кратности интервалов таймеров для всего приложения
	передаётся новому экземпляру, старый экземпляр лишается этого права.
	*/
	timer& operator = ( timer&& other ) noexcept
	{
		interval = other.interval;
		time_point = other.time_point;
		other.reset_tick_interval();
		return *this;
	}

	/*
	@return Текущая длительность интервалов в миллисекундах.
	*/
	ms::rep get_interval() const noexcept { return interval.count(); }

	/*
	Устанавливает длительность интервалов в миллисекундах.

	@param new_interval - новая длительность интервалов в миллисекундах.
	*/
	void set_interval( ms::rep new_interval ) noexcept { interval = ms { new_interval }; }

	/*
	Изменяет точку отсчёта на "текущую + интервал".

	@return Новая точка отсчёта.
	*/
	clock::time_point from_now() noexcept
	{
		time_point = clock::now() + interval;
		return time_point;
	}

	/*
	Изменяет точку отсчёта на "предыдущую + интервал".

	@return Новая точка отсчёта.
	*/
	clock::time_point from_prev() noexcept
	{
		time_point += interval;
		return time_point;
	}

	/*
	@return Минимально и максимално возможные значения
	кратности интервалов таймеров в миллисекундах.
	*/
	static timecaps get_timecaps();

	/*
	Устанавливает новую кратноть интервалов таймеров для всего приложения. 
	Выбирается наименьшая кратность из всех действующих на данный момент. 
	Значения, выходящие за диапазон, возвращаемый методом get_timecaps(), 
	всё равно будут вписаны в этот диапазон.

	@param tick_interval - новая кратность. Установка значения в 0 
	приравнивается к сбросу установки кратности данным экземпляром.
	*/
	void set_tick_interval( unsigned int tick_interval );

	/*
	Сброс установки кратности интервалов таймеров для всего приложения.
	*/
	void reset_tick_interval() noexcept;

	/*
	Вызывает метод reset_tick_interval() при разрушении.
	*/
	~timer()
	{
		reset_tick_interval();
	}

private:
	ms interval { 15 };
	clock::time_point time_point = clock::now();
	unsigned int tick_interval = 0;
};



} // namespace tools
} // namespace engine
} // namespace heimdall

#endif // !HEIMDALL_ENGINE_TOOLS_TIMER_HPP