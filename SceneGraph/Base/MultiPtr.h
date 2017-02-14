#pragma once

#include <cstddef>
#include <memory>

namespace kitsune {
namespace scenegraph {

	enum class delete_condition {
		all,
		any
	};

	template <class T, delete_condition C = delete_condition::any, class D = std::default_delete<T>>
	class MultiPtr
	{
	public:
		typedef MultiPtr<T, C, D> _Myt;
		typedef std::unique_ptr<T> child_ptr;
		typedef T* pointer;
		static const delete_condition condition = C;
		
		constexpr MultiPtr() noexcept : ptr(nullptr), count(0)
		{
		}

		constexpr MultiPtr(std::nullptr_t) noexcept : MultiPtr()
		{
		}

		explicit MultiPtr(pointer p) noexcept : ptr(p)
		{
			count = 0;
		}

		MultiPtr(pointer p, typename std::conditional<std::is_reference<D>::value, D, const D&> del) noexcept : ptr(p), deleter(del)
		{
			count = 0;
		}

		MultiPtr(pointer p, typename std::remove_reference<D>::type && del) noexcept : ptr(p), deleter(del)
		{
			count = 0;
		}

		MultiPtr(MultiPtr && p) noexcept
		{
			count = p.count;
			ptr = p.ptr;
			deleter = p.deleter;
			p.ptr = nullptr;
		}

		MultiPtr(const MultiPtr&) = delete;
		_Myt& operator=(const _Myt&) = delete;

		~MultiPtr()
		{
			if (get() != pointer())
				deleter(get());
		}

		typename std::add_lvalue_reference<T>::type operator*() const
		{
			return *get();
		}

		pointer operator->() const noexcept
		{
			return std::pointer_traits<pointer>::pointer_to(*this);
		}

		pointer get() const noexcept
		{
			return ptr;
		}

		D& get_deleter() noexcept
		{
			return deleter;
		}

		const D& get_deleter() const noexcept
		{
			return deleter;
		}

		void reset(pointer p = pointer()) noexcept
		{
			pointer old = get();
			this->ptr = p;
			if (old != pointer())
				deleter(old);
			count = this->ptr ? 1 : 0;
		}

		bool expired() const noexcept
		{
			return count == 0;
		}

		explicit operator bool() const noexcept
		{
			return !expired() && get() != pointer();
		}

		template <delete_condition U = C>
		typename std::enable_if<U == delete_condition::any, child_ptr>::type add_child() noexcept
		{
			return child_ptr(get(), [this](T* ptr) {
				this->reset();
			});
		}

		template <delete_condition U = C>
		typename std::enable_if<U == delete_condition::all, child_ptr>::type add_child() noexcept
		{
			++count;
			return child_ptr(get(), [this](T* ptr) {
				if (--count == 0)
					this->reset();
			});
		}

	private:
		pointer ptr;
		D deleter;
		int count;
	};

}
}

