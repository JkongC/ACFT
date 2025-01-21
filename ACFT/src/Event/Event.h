#ifndef ACFT_EVENT_H_
#define ACFT_EVENT_H_

namespace ACFT
{
	class Event
	{
	public:
		enum class Type
		{
			key_press = 0, key_release, 
			mouse_move, mouse_keydown, mouse_keyup,
			mouse_scroll
		};

		Event(Type type);

		const Type& GetType() const { return type; }

	private:
		Type type;
	};

	class InputEvent : public Event
	{
	public:
		InputEvent(Type type);

		union
		{
			struct
			{
				int keycode;
				bool down;
			};

			struct
			{
				double xpos;
				double ypos;
			};

			struct
			{
				double xoffset;
				double yoffset;
			};
		};

	};
}

#endif