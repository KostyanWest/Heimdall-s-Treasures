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
Хранит минимально и максимально возможные значения 
кратности интервалов таймеров в миллисекундах.
*/
struct timecaps
{
	unsigned int min_period;
	unsigned int max_period;
};



/*
Устанавливает кратность интервалов таймеров для всего приложения в миллисекундах.
Для установки кратности создайте экземпляр класса. Для сброса - уничтожьте.
Выбирается наименьшее значение из существующих на данный момент.
Значение кратности в 0 миллисекунд не оказывает влияния.
*/
class time_period
{
public:
	/*
	Значение по умолчанию: 0. Не оказывает влияния.
	*/
	time_period() noexcept {}

	/*
	Значение кратности в 0 миллисекунд не оказывает влияния. 
	Значения, выходящие за диапазон, возвращаемый методом get_timecaps(), 
	всё равно будут вписаны в этот диапазон.

	@param interval - новое значение кратности для данного экземпляра.
	*/
	time_period( unsigned int interval );

	time_period( const time_period& other ) noexcept;

	time_period( time_period&& other ) noexcept
		: interval( other.interval )
	{
		other.interval = 0;
	}

	time_period& operator = ( const time_period& other ) noexcept
	{
		time_period copy( other );
		std::swap( interval, copy.interval );
		return *this;
	}

	time_period& operator = ( time_period&& other ) noexcept
	{
		time_period move( std::move( other ) );
		std::swap( interval, move.interval );
		return *this;
	}

	operator unsigned int() noexcept { return interval; }

	/*
	Сбрасывает кратность, установленную данным экземпляром.
	*/
	~time_period();

	/*
	@return Минимально и максимално возможные значения
	кратности интервалов таймеров в миллисекундах.
	*/
	static timecaps get_timecaps();

private:
	unsigned int interval = 0;
};



/*
Удобный класс для отсчёта одинаковых интервалов времени.
*/
class timer
{
private:
	using clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::milliseconds;

public:
	/*
	Создаёт таймер с заданными настройками.

	@param time_point - начальная точка отсчёта.
	@param interval - длительность интервалов в миллисекундах, которые будет отсчитывать таймер.
	*/
	timer( clock::time_point time_point = clock::now(), ms::rep interval = 15 ) noexcept
		: time_point( time_point ), interval( interval )
	{
	}

	/*
	@return Предыдущая точка отсчёта.
	*/
	explicit operator clock::time_point() const noexcept { return time_point; }

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
	@return Разница в миллисекундах между предыдущей и текущей точками отсчёта.
	*/
	ms::rep diff() const noexcept { return (time_point - clock::now()).count(); }


	/*
	Устанавливает точку отсчёта на текущую.
	*/
	void to_now() noexcept { time_point = clock::now(); }

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
	clock::time_point from_last() noexcept
	{
		time_point += interval;
		return time_point;
	}

private:
	clock::time_point time_point = clock::now();
	ms interval { 15 };
};



} // namespace tools
} // namespace engine
} // namespace heimdall

#endif // !HEIMDALL_ENGINE_TOOLS_TIMER_HPP
