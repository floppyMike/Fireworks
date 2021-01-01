#pragma once

#include <CustomLibrary/SDL/Renderer.h>
#include <CustomLibrary/SDL/Render.h>
#include <CustomLibrary/SDL/Window.h>
#include <CustomLibrary/RandomGenerator.h>

#include "Particle.h"

using namespace ctl;

static rnd::Random<rnd::Mersenne> g_rand;

class ParticlePack
{
public:
	static constexpr int FADE = 1;

	void update() noexcept
	{
		if (m_color.a <= FADE)
			m_color.a = 0;
		else
			m_color.a -= FADE;

		for (auto [i, j] = std::pair{ m_spots.begin(), m_attrib.begin() }; i != m_spots.end(); ++i, ++j) j->update(&*i);
	}

	void explode(const SDL_Color &c, mth::Point<float> p)
	{
		const auto n = g_rand.rand_number(10, 100);
		m_color		 = c;

		m_spots = std::vector<Particle>(n, p);

		m_attrib.reserve(n);
		while (m_attrib.capacity() != m_attrib.size())
			m_attrib.emplace_back(g_rand.rand_number(-1.F, 1.F), g_rand.rand_number(-1.F, 1.F));
	}

	void reset()
	{
		m_spots.clear();
		m_attrib.clear();
	}

	constexpr auto fade() const noexcept { return m_color.a; }
	auto		   exploded() const noexcept -> bool { return !m_spots.empty(); }

	friend auto operator<<(sdl::Renderer &r, const ParticlePack &pack)
	{
		std::span s(pack.m_spots);
		r.color(pack.m_color);
		sdl::draw(&s, &r).points();
	}

private:
	std::vector<Particle>			m_spots;
	std::vector<ParticleAttributes> m_attrib;
	SDL_Color						m_color;
};

class Firework
{
public:
	explicit Firework(sdl::Window *w)
		: m_win(w)
	{
		reset();
	}

	auto update() -> bool
	{
		if (!m_pack.exploded())
		{
			m_attrib.update(&m_spot);
			if (m_attrib.vely() <= 0)
				m_pack.explode({ g_rand.rand_number<Uint8>(90, 255), g_rand.rand_number<Uint8>(90, 255),
								 g_rand.rand_number<Uint8>(90, 255), 255 },
							   m_spot);
		}
		else
		{
			m_pack.update();
			if (m_pack.fade() == 0)
				if (m_win == nullptr)
					return true;
				else
					reset();
		}

		return false;
	}

	void reset()
	{
		m_spot = { g_rand.rand_number(m_win->dim().w / 4.F, m_win->dim().w * 3 / 4.F), m_win->dim().h };
		m_attrib.reset(g_rand.rand_number(-.5F, .5F), g_rand.rand_number(2.F, 3.5F));
		m_pack.reset();
	}

	void kill() { m_win = nullptr; }

	friend auto operator<<(sdl::Renderer &r, const Firework &f) -> sdl::Renderer &
	{
		if (!f.m_pack.exploded())
		{
			r.color({ 100, 100, 100, 0xFF });
			sdl::draw(&f.m_spot, &r).point();
		}
		else
			r << f.m_pack;

		return r;
	}

private:
	sdl::Window *m_win;

	Particle		   m_spot;
	ParticleAttributes m_attrib;
	ParticlePack	   m_pack;
};

class Fireworks
{
public:
	Fireworks()
		: m_win("Fireworks Simulation", { 800, 800 }, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
		, m_rend(&m_win)
	{
		m_rend.blend_mode(SDL_BLENDMODE_BLEND);
		m_fireworks.emplace_back(&m_win);
	}

	void pre_pass() {}

	void update()
	{
		bool kill = true;

		for (auto i = m_fireworks.rbegin(); i != m_fireworks.rend(); ++i)
			if (i->update() && kill)
			{
				--m_kill;
				m_fireworks.pop_back();
			}
			else
				kill = false;
	}

	void event(const SDL_Event &e)
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_UP)
				m_fireworks.emplace_back(&m_win);
			if (e.key.keysym.sym == SDLK_DOWN)
			{
				if (m_kill != m_fireworks.size())
					(m_fireworks.rbegin() + m_kill++)->kill();
			}
			break;
		}
	}

	void render()
	{
		sdl::render(&m_rend).fill(sdl::BLACK);
		for (const auto &f : m_fireworks) m_rend << f;
		sdl::render(&m_rend).render();
	}

private:
	sdl::Window	  m_win;
	sdl::Renderer m_rend;

	std::vector<Firework> m_fireworks;
	size_t				  m_kill = 0;
};