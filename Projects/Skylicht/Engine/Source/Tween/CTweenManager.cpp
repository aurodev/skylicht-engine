#include "pch.h"
#include "CTweenManager.h"

namespace Skylicht
{
	CTweenManager::CTweenManager()
	{

	}

	CTweenManager::~CTweenManager()
	{

	}

	void CTweenManager::update()
	{
		m_tweens.insert(m_tweens.end(), m_insert.begin(), m_insert.end());
		m_insert.clear();

		for (CTween* tween : m_tweens)
		{
			tween->update();
		}

		for (CTween* tween : m_remove)
		{
			std::vector<CTween*>::iterator i = std::find(m_tweens.begin(), m_tweens.end(), tween);
			if (i != m_tweens.end())
			{
				m_tweens.erase(i);
			}
			delete tween;
		}
		m_remove.clear();
	}

	void CTweenManager::addTween(CTween* tween)
	{
		m_insert.push_back(tween);
	}

	void CTweenManager::removeTween(CTween* tween)
	{
		m_remove.push_back(tween);
	}
}