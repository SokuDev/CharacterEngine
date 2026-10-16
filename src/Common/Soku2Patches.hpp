//
// Created by PinkySmile on 05/06/2026.
//

#ifndef CHARACTERENGINE_SOKU2PATCHES_HPP
#define CHARACTERENGINE_SOKU2PATCHES_HPP


#include <cstdint>
#include <stdexcept>

struct JumpTarget {
	size_t patchOffset;
	size_t targetAddress;
};

struct PatchByteCode {
	unsigned char *byteCode;
	size_t byteCodeLength;
	JumpTarget *jumps;
	size_t jumpsSize;
};

struct PatchSkeleton {
	PatchByteCode &byteCode;
	void *location;
	size_t patchSize;
};

template<size_t arenaSize>
class StackedMemory {
private:
#ifdef _DEBUG
	unsigned char _buffer[arenaSize + 80];
#else
	unsigned char _buffer[arenaSize];
#endif
	unsigned char *_nextPtr = this->_buffer;

public:
	unsigned char *alloc(size_t size)
	{
		if (this->_nextPtr + size > std::end(this->_buffer)) {
			puts("Out of memory!");
			printf("Trying to allocate %zu bytes but only %zu are remaining.\n", size, sizeof(this->_buffer) - this->getIndex());
			throw std::bad_alloc();
		}

		unsigned char *ptr = this->_nextPtr;

		this->_nextPtr += size;
		return ptr;
	}

	void dealloc(unsigned char *newEnd)
	{
		if (newEnd > std::end(this->_buffer) || newEnd < std::begin(this->_buffer)) {
			printf("Invalid dealloc: %p < %p < %p\n", std::begin(this->_buffer), newEnd, std::end(this->_buffer));
			throw std::bad_alloc();
		}
		if (this->_nextPtr > newEnd)
			this->_nextPtr = newEnd;
	}

	void reset() noexcept
	{
		this->_nextPtr = this->_buffer;
	}

	size_t getIndex() const noexcept
	{
		return this->_nextPtr - this->_buffer;
	}

	unsigned char *getBufferStart()
	{
		return this->_buffer;
	}

	size_t size()
	{
		return sizeof(this->_buffer);
	}
};

template<typename T, size_t arenaSize>
class StackedAllocator {
public:
	std::reference_wrapper<StackedMemory<arenaSize>> _memory;

	StackedAllocator(StackedMemory<arenaSize> &memory) :
		_memory(memory)
	{
	}

	template<typename U>
	StackedAllocator(const StackedAllocator<U, arenaSize> &other) noexcept :
		_memory(other._memory)
	{
	}

	T *allocate(size_t size)
	{
		return reinterpret_cast<T *>(this->_memory.get().alloc(size * sizeof(T)));
	}
	void deallocate(T *ptr, size_t)
	{
		this->_memory.get().dealloc(reinterpret_cast<unsigned char *>(ptr));
	}

	template <typename U> struct rebind { typedef StackedAllocator<U, arenaSize> other; };

	typedef T value_type;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef T &reference;
	typedef const T &const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
};

struct Patch {
	void *location = nullptr;
	size_t patchSize = 0;
	unsigned char *trampoline = nullptr;

	Patch(const PatchSkeleton &skeleton);

	Patch(Patch &o);
	Patch(Patch &&o) noexcept;
	Patch &operator=(Patch &&o) noexcept;
};

class AppliedPatch {
private:
	const Patch *_patch = nullptr;
	unsigned char *_oldData = nullptr;

public:
	AppliedPatch(const Patch *patch);
	~AppliedPatch();

	AppliedPatch(AppliedPatch &o);
	AppliedPatch(AppliedPatch &&o) noexcept;
	AppliedPatch &operator=(AppliedPatch &&o) noexcept;
};

static_assert(sizeof(AppliedPatch) == 8);
static_assert(sizeof(Patch) == 12);


#endif //CHARACTERENGINE_SOKU2PATCHES_HPP
