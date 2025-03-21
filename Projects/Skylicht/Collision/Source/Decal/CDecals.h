/*
!@
MIT License

Copyright (c) 2022 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the Rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#pragma once

#include "Components/CComponentSystem.h"
#include "Entity/CEntityHandler.h"

#include "CDecalsRenderer.h"
#include "CDecalData.h"

#include "Collision/CCollisionBuilder.h"

namespace Skylicht
{
	class CDecals : public CEntityHandler
	{
	protected:
		CDecalRenderData* m_renderData;

	public:
		CDecals();

		virtual ~CDecals();

		virtual void initComponent();

		virtual void updateComponent();

		virtual CObjectSerializable* createSerializable();

		virtual void loadSerializable(CObjectSerializable* object);

		void setTexture(ITexture* texture);

		CEntity* addDecal(
			const core::vector3df& position,
			const core::vector3df& dimension,
			const core::vector3df& normal,
			float textureRotation,
			float lifeTime,
			float distance);

		void bake(CCollisionBuilder* collisionMgr);

		DECLARE_GETTYPENAME(CDecals);

	protected:

		void initDecal(CEntity* entity, CDecalData* decal, const core::vector3df& position, CCollisionBuilder* collision);
	};
}