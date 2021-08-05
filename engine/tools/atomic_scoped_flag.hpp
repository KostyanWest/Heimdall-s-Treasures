#ifndef HEIMDALL_ENGINE_TOOLS_ATOMIC_SCOPED_FLAG_HPP
#define HEIMDALL_ENGINE_TOOLS_ATOMIC_SCOPED_FLAG_HPP

#include <atomic>



namespace heimdall
{
namespace engine
{
namespace tools
{



/*
Работает аналогично std::atomic_flag, но вызывает метод clear() при 
выходе из области видимости, если до этого успешно завладел флагом.

@param T_atomic - любой std::atomic<>, поддерживающий работу с bool переменными.
@param B_set - состояние флага, при котором флаг считается поднятым.
@param A_order - модель памяти для операции чтения-изменения-записи own().
@param R_order - модель памяти для операций записи clear() и деструктора.

@exception noexcept
*/
template<
	typename T_atomic,
	bool B_set = true,
	std::memory_order A_order = std::memory_order_acquire,
	std::memory_order R_order = std::memory_order_release
>
class atomic_scoped_flag
{
public:
	/*
	Связывает объект с атомиком-флагом.
	*/
	explicit atomic_scoped_flag( T_atomic& atomic ) noexcept
		: atomic( atomic )
	{
	};

	atomic_scoped_flag( const atomic_scoped_flag& ) = delete;
	atomic_scoped_flag& operator = ( const atomic_scoped_flag& ) = delete;

	/*
	Завладевает флагом, если у флага ещё не было владельца. 
	Операция чтения-изменения-записи.

	@return true, если владение флагом установлено,
	false, если у флага уже есть владелец.
	*/
	bool own() noexcept
	{
		if (!is_owner)
			is_owner = atomic.exchange( B_set, A_order ) != B_set;
		return is_owner;
	}

	/*
	@return true, если объект владеет флагом, false в ином случае.
	*/
	bool get_is_owner() const noexcept
	{
		return is_owner;
	}

	/*
	Если объект владел флагом, прекращает им владеть. Операция записи.
	*/
	void clear() noexcept
	{
		if (is_owner)
			atomic.store( !B_set, R_order );
	}

	/*
	Прекращает владеть каким-либо флагом при уничтожении. Операция записи.
	*/
	~atomic_scoped_flag()
	{
		clear();
	}

private:
	T_atomic& atomic;
	bool is_owner = false;
};



/*
Работает аналогично std::atomic_flag, но вызывает метод clear() при
выходе из области видимости, если до этого успешно завладел флагом.

@param A_order - модель памяти для операции чтения-изменения-записи own().
@param R_order - модель памяти для операций записи clear() и деструктора.

@exception noexcept
*/
template<
	std::memory_order A_order,
	std::memory_order R_order
>
class atomic_scoped_flag<std::atomic_flag, true, A_order, R_order>
{
public:
	explicit atomic_scoped_flag( std::atomic_flag& atomic ) noexcept
		: atomic( atomic )
	{
	};

	atomic_scoped_flag( const atomic_scoped_flag& ) = delete;
	atomic_scoped_flag& operator = ( const atomic_scoped_flag& ) = delete;

	bool own() noexcept
	{
		if (!is_owner)
			is_owner = !atomic.test_and_set( A_order );
		return is_owner;
	}

	bool get_is_owner() const noexcept
	{
		return is_owner;
	}

	void clear() noexcept
	{
		if (is_owner)
			atomic.clear( R_order );
	}

	~atomic_scoped_flag()
	{
		clear();
	}

private:
	std::atomic_flag& atomic;
	bool is_owner = false;
};



} // namespace tools
} // namespace engine
} // namespace heimdall

#endif // !HEIMDALL_ENGINE_TOOLS_ATOMIC_SCOPED_FLAG_HPP
