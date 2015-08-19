#pragma once
#include <functional>
#include <list>
#include <map>
#include <algorithm>

namespace kitsune {
namespace scenegraph {

	template<class> class callback
	{};

	template<class> class auto_callback
	{};

	namespace helper {
		template <class CB>
		class auto_remover {
		private:
			int functionId;
			CB* callback;

		public:
			auto_remover(int id, CB* callback)
				: functionId(id), callback(callback)
			{}
			~auto_remover() {
				callback->pop(functionId);
			}
		};
	}

	namespace nonvoid {
		template <class R, typename... ArgTypes>
		class caller {
		public:
			typedef std::function<R(ArgTypes...)> function_type;

			std::map<int, R> operator()(std::map<int, function_type> &functions, ArgTypes... args) {
				std::map<int, R> r;

				for (auto &&f : functions) {
					r[f.first] = f.second(args...);
				}

				return r;
			}
		};
	}

	template<class R, typename... ArgTypes>
	class callback<R(ArgTypes...)>
	{
	public:
		typedef class callback<R(ArgTypes...)> _Myt;
		typedef R result_type;
		typedef std::function<R(ArgTypes...)> function_type;
		typedef class helper::auto_remover<_Myt> auto_remover_type;

	private:
		nonvoid::caller<R, ArgTypes...> caller;

	public:
		callback()
		{
			m_id = 0;
		}

		_Myt& clear() {
			m_id = 0;
			m_functions.clear();
			return *this;
		}

		operator bool() const {
			return !m_functions.empty();
		}

		int push(const function_type &fn) {
			m_functions[++m_id] = fn;

			return m_id;
		}

		int push(function_type && fn) {
			m_functions[++m_id] = fn;

			return m_id;
		}

		_Myt& pop(int index) {
			auto i = m_functions.find(index);
			if (i != m_functions.end()) m_functions.erase(i);

			return *this;
		}

		std::map<int, R> operator() (ArgTypes... args) {
			return caller(m_functions, args...);
		}

		_Myt& operator-= (int index) {
			this->pop(index);

			return *this;
		}

		_Myt& operator+= (function_type && fn) {
			this->push(fn);

			return *this;
		}

		_Myt& operator= (function_type && fn) {
			this->clear();
			this->push(fn);

			return *this;
		}

	private:
		int m_id;
		std::map<int, function_type> m_functions;
	};

	namespace noret {
		template <typename... ArgTypes>
		class caller {
		public:
			typedef std::function<void(ArgTypes...)> function_type;

			void operator()(std::map<int, function_type> &functions, ArgTypes... args) {
				for (auto &f : functions) {
					f.second(args...);
				}
			}
		};
	}

	template<class... ArgTypes>
	class callback<void(ArgTypes...)>
	{
	public:
		typedef class callback<void(ArgTypes...)> _Myt;
		typedef void result_type;
		typedef std::function<void(ArgTypes...)> function_type;

	private:
		noret::caller<ArgTypes...> caller;

	public:
		callback()
		{
			m_id = 0;
		}

		_Myt& clear() {
			m_id = 0;
			m_functions.clear();
			return *this;
		}

		operator bool() const {
			return !m_functions.empty();
		}

		void operator() (ArgTypes... args) {
			caller(m_functions, args...);
		}

		int push(const function_type &fn) {
			m_functions[++m_id] = fn;

			return m_id;
		}

		int push(function_type && fn) {
			m_functions[++m_id] = fn;

			return m_id;
		}

		void pop(int index) {
			auto i = m_functions.find(index);
			if (i != m_functions.end()) m_functions.erase(i);
		}

		_Myt& operator-= (int index) {
			this->pop(index);

			return *this;
		}

		_Myt& operator+= (function_type && fn) {
			this->push(fn);

			return *this;
		}

		_Myt& operator= (function_type && fn) {
			this->clear();
			this->push(fn);

			return *this;
		}

	private:
		int m_id;
		std::map<int, function_type> m_functions;
	};
	
	template<class R, typename... ArgTypes>
	class auto_callback<R(ArgTypes...)>
		: public callback<R(ArgTypes...)>
	{
	public:
		typedef class auto_callback<R(ArgTypes...)> _Myt;
		typedef class helper::auto_remover<_Myt> auto_remover_type;

		auto_remover_type push_auto(const function_type &fn) {
			return auto_remover_type(this->push(fn), this);
		}

		auto_remover_type push_auto(function_type && fn) {
			return auto_remover_type(this->push(fn), this);
		}
	};

	template<class... ArgTypes>
	class auto_callback<void(ArgTypes...)>
		: public callback<void(ArgTypes...)>
	{
	public:
		typedef class auto_callback<void(ArgTypes...)> _Myt;
		typedef class helper::auto_remover<_Myt> auto_remover_type;

		auto_remover_type push_auto(const function_type &fn) {
			return auto_remover_type(this->push(fn), this);
		}

		auto_remover_type push_auto(function_type && fn) {
			return auto_remover_type(this->push(fn), this);
		}
	};
}
}
