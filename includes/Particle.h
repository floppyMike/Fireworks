#pragma once

#include <CustomLibrary/Point.h>
#include <CustomLibrary/SDL/Drawable.h>
#include <CustomLibrary/SDL/Window.h>

using namespace ctl;

using Particle = mth::Point<float>;

class ParticleAttributes
{
public:
	static constexpr float GRAVITY = -.01F;

	ParticleAttributes() = default;

	ParticleAttributes(float velx, float vely) { reset(velx, vely); }

	void update(mth::Point<float> *p)
	{
		m_vely += m_accy;

		p->x -= m_velx;
		p->y -= m_vely;
	}

	void reset(float velx, float vely)
	{
		m_velx = velx;
		m_vely = vely;
	}

	constexpr auto vely() const noexcept { return m_vely; }

private:
	float m_velx = 0, m_vely = 0;
	float m_accy = GRAVITY;
};