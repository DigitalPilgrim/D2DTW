#pragma once

class Holder
{
public:
	virtual ~Holder() {}

	virtual void Update() = 0;
};

template <typename T>
class HolderMeta : public Holder
{
	using function = void(T::*)(void);

	T* m_caller;
	function m_function;
public:
	HolderMeta() : m_caller(nullptr), m_function(nullptr) {}
	HolderMeta(T* caller, function f) : m_caller(caller), m_function(f) {}

	void Update() override
	{
		if (m_caller != nullptr && m_function != nullptr)
		{
			(m_caller->*m_function)();
		}
	}
};

class UpdateHolder
{
	Holder* holder;
public:
	UpdateHolder() : holder(nullptr) {}

	template <typename T>
	void Add(T * caller, void(T::*f)(void)) 
	{
		holder = new HolderMeta<T>(caller, f);
	}

	void Update()
	{
		if (holder != nullptr)
		{
			holder->Update();
		}
	}

	~UpdateHolder()
	{
		if (holder != nullptr)
		{
			delete holder;
		}
	}

};