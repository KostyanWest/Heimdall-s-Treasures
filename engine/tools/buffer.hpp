#ifndef HEIMDALL_ENGINE_TOOLS_BUFFER_HPP
#define HEIMDALL_ENGINE_TOOLS_BUFFER_HPP



namespace heimdall
{
namespace engine
{
namespace tools
{



/*
Простейщее представления буфера.

@exception noexcept
*/
struct buffer
{
	buffer( void* ptr, size_t size ) noexcept
		: ptr( ptr ), size( size )
	{
	}

	explicit operator bool() const noexcept { return ptr && size; }

	void* ptr;
	size_t size;
};



} // namespace tools
} // namespace engine
} // namespace heimdall

#endif // !HEIMDALL_ENGINE_TOOLS_BUFFER_HPP
