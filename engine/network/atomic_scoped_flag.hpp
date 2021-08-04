#ifndef HEIMDALL_ENGINE_NETWORK_ATOMIC_SCOPED_FLAG_HPP
#define HEIMDALL_ENGINE_NETWORK_ATOMIC_SCOPED_FLAG_HPP

#include <atomic>



namespace heimdall
{
namespace engine
{
namespace network
{



/*
�������� ���������� std::atomic_flag, �� �������� ����� clear() ��� 
������ �� ������� ���������, ���� �� ����� ������� �������� ������.

@param T_atomic - ����� std::atomic<>, �������������� ������ � bool �����������.
@param A_order - ������ ������ ��� �������� ������-���������-������ own().
@param R_order - ������ ������ ��� �������� ������ clear() � �����������.
*/
template<
	typename T_atomic,
	std::memory_order A_order = std::memory_order_acquire,
	std::memory_order R_order = std::memory_order_release
>
class atomic_scoped_flag
{
public:
	/*
	��������� ������ � ��������-������.
	*/
	explicit atomic_scoped_flag( T_atomic& atomic ) noexcept
		: atomic( atomic )
	{
	};

	atomic_scoped_flag( const atomic_scoped_flag& ) = delete;
	atomic_scoped_flag& operator = ( const atomic_scoped_flag& ) = delete;

	/*
	����������� ������, ���� � ����� ��� �� ���� ���������. 
	�������� ������-���������-������.

	@return true, ���� �������� ������ �����������,
	false, ���� � ����� ��� ���� ��������.
	*/
	bool own() noexcept
	{
		if (!is_owner)
			is_owner = !atomic.exchange( true, A_order );
		return is_owner;
	}

	/*
	@return true, ���� ������ ������� ������, false � ���� ������.
	*/
	bool get_is_owner() const noexcept
	{
		return is_owner;
	}

	/*
	���� ������ ������ ������, ���������� �� �������. �������� ������.
	*/
	void clear() noexcept
	{
		if (is_owner)
			atomic.store( false, R_order );
	}

	/*
	���������� ������� �����-���� ������ ��� �����������. �������� ������.
	*/
	~atomic_scoped_flag() noexcept
	{
		clear();
	}

private:
	T_atomic& atomic;
	bool is_owner = false;
};



/*
�������� ���������� std::atomic_flag, �� �������� ����� clear() ���
������ �� ������� ���������, ���� �� ����� ������� �������� ������.

@param A_order - ������ ������ ��� �������� ������-���������-������ own().
@param R_order - ������ ������ ��� �������� ������ clear() � �����������.
*/
template<
	std::memory_order A_order,
	std::memory_order R_order
>
class atomic_scoped_flag<std::atomic_flag, A_order, R_order>
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

	~atomic_scoped_flag() noexcept
	{
		clear();
	}

private:
	std::atomic_flag& atomic;
	bool is_owner = false;
};



} // namespace network
} // namespace engine
} // namespace heimdall

#endif // !HEIMDALL_ENGINE_NETWORK_ATOMIC_SCOPED_FLAG_HPP
