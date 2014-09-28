#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include <map>

/// State Machine
/// E = a user defined enum/index for a unique state
/// T = the class that owns the state machine
template<class E, class T>
class State_Machine
{
public:
	typedef void(T::*callback_on_enter)();
	typedef void(T::*callback_on_update)(float);
	typedef void(T::*callback_on_render)();
	typedef void(T::*callback_on_exit)();

private:
	T* machine_owner;
	E current_state;

	std::map<E, callback_on_enter> states_on_enter;
	std::map<E, callback_on_update> states_on_update;
	std::map<E, callback_on_render> states_on_render;
	std::map<E, callback_on_exit> states_on_exit;

public:
	State_Machine(T* machine_owner, E empty_state)
	{
		current_state = empty_state;
		this->machine_owner = machine_owner;
	}

	~State_Machine()
	{
	}

	void register_state(E state,
						callback_on_enter callback_entry,
						callback_on_update callback_update,
						callback_on_render callback_render,
						callback_on_exit callback_exit)
	{
		states_on_enter[state] = callback_entry;
		states_on_update[state] = callback_update;
		states_on_render[state] = callback_render;
		states_on_exit[state] = callback_exit;
	}

	E get_current_state(void)
	{
		return current_state;
	}

	void change_state(E state_next)
	{
		if(machine_owner)
		{
			callback_on_exit callback_exit = states_on_exit[current_state];
			if(callback_exit)
			{
				//Exit old state
				(machine_owner->*callback_exit)();
			}
		}

		current_state = state_next;

		if(machine_owner)
		{
			callback_on_enter callback_enter = states_on_enter[current_state];
			if(callback_enter)
			{
				//Enter new state
				(machine_owner->*callback_enter)();
			}
		}
	}

	//Run the update command for current state
	void update(float timestep)
	{
		if(machine_owner)
		{
			callback_on_update callback = states_on_update[current_state];
			if(callback)
			{
				(machine_owner->*callback)(timestep);
			}
		}
	}

	//Run the render command for current state
	void render()
	{
		if(machine_owner)
		{
			callback_on_render callback = states_on_render[current_state];
			if(callback)
			{
				(machine_owner->*callback)();
			}
		}
	}
};
#endif