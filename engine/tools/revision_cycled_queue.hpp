#ifndef HEIMDALL_ENGINE_TOOLS_REVISION_CYCLED_QUEUE_HPP
#define HEIMDALL_ENGINE_TOOLS_REVISION_CYCLED_QUEUE_HPP

#include <atomic>



namespace heimdall
{
namespace engine
{
namespace tools
{



template<typename T, size_t N>
class revision_cycled_queue
{
public:
	revision_cycled_queue() noexcept {}

	revision_cycled_queue( const revision_cycled_queue& ) = delete;
	revision_cycled_queue& operator = ( const revision_cycled_queue& ) = delete;

	bool try_push(T&& object) noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		T* current_end = end.load( std::memory_order_relaxed );
		T* next_end = inc( current_end );

		if (next_end != begin.load( std::memory_order_acquire ))
		{
			new (current_end) T( object );
			end.store( next_end, std::memory_order_release );
			return true;
		}
		else
		{
			return false;
		}
	}

	bool try_pop(T& object) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>)
	{
		T* current_begin = begin.load( std::memory_order_relaxed );

		if (current_begin != end.load( std::memory_order_acquire ))
		{
			object = std::move( *current_begin );
			current_begin->~T();
			begin.store( inc( current_begin ), std::memory_order_release );
			return true;
		}
		else
		{
			return false;
		}
	}

	~revision_cycled_queue() noexcept(std::is_nothrow_destructible_v<T>)
	{
		T* begin_ptr = begin.load( std::memory_order_relaxed );
		T* end_ptr = end.load( std::memory_order_relaxed );
		while (begin_ptr != end_ptr)
		{
			begin_ptr->~T();
			begin_ptr = inc( begin_ptr );
		}
	}

private:
	T* inc( T* ptr ) noexcept
	{
		ptr++;
		if (ptr == reinterpret_cast<T*>(buf) + N)
			ptr = reinterpret_cast<T*>(buf);
		return ptr;
	}

	char buf[sizeof( T ) * N];
	std::atomic<T*> begin = reinterpret_cast<T*>(buf);
	std::atomic<T*> end = reinterpret_cast<T*>(buf);
};



} // namespace tools
} // namespace engine
} // namespace heimdall

#endif // !HEIMDALL_ENGINE_TOOLS_REVISION_CYCLED_QUEUE_HPP