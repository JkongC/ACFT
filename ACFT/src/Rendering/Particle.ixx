export module ACFT.Particle;

import Types;
import Event;
import ACFT.PhysicsComponents;

namespace ACFT
{
	class Particle2D
	{
	public:
		Particle2D();
		Particle2D(PositionComponent2D pos, MassComponent m_Mass, SpeedComponent2D speed = {});

		void OnUpdate(float time_step);
		virtual void OnRender() = 0;
		virtual void OnEvent(Ref<Event> event) = 0;

	private:
		PositionComponent2D m_Pos;
		MassComponent m_Mass;
		SpeedComponent2D m_Speed;
	};

	class Particle3D
	{
	public:
		Particle3D();
		Particle3D(PositionComponent3D pos, MassComponent m_Mass, SpeedComponent3D speed = {});

		void OnUpdate(float time_step);
		virtual void OnRender() = 0;
		virtual void OnEvent(Ref<Event> event) = 0;

	private:
		PositionComponent3D m_Pos;
		MassComponent m_Mass;
		SpeedComponent3D m_Speed;
	};
}
