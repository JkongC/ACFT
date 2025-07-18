module ACFT.Particle;

namespace ACFT
{
	Particle2D::Particle2D()
		: m_Pos(), m_Mass(), m_Speed()
	{ }

	Particle2D::Particle2D(PositionComponent2D pos, MassComponent mass, SpeedComponent2D speed)
		: m_Pos(pos), m_Mass(mass), m_Speed(speed)
	{ }

	void Particle2D::OnUpdate(float time_step)
	{
		m_Pos.x += m_Speed.x * time_step;
		m_Pos.y += m_Speed.y * time_step;
	}

	Particle3D::Particle3D()
		: m_Pos(), m_Mass(), m_Speed()
	{
	}

	Particle3D::Particle3D(PositionComponent3D pos, MassComponent mass, SpeedComponent3D speed)
		: m_Pos(pos), m_Mass(mass), m_Speed(speed)
	{
	}

	void Particle3D::OnUpdate(float time_step)
	{
		m_Pos.x += m_Speed.x * time_step;
		m_Pos.y += m_Speed.y * time_step;
		m_Pos.z += m_Speed.z * time_step;
	}
}
