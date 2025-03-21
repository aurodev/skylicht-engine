/*
!@
MIT License

Copyright (c) 2021 Skylicht Technology CO., LTD

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

#include "Camera/CCamera.h"
#include "Entity/IEntityData.h"
#include "LineDraw/CLineDrawData.h"

namespace Skylicht
{
	namespace Editor
	{
		class CViewpointData :
			public CLineDrawData,
			public IEntityData
		{
		public:
			enum EAxis
			{
				None = -1,
				X = 0,
				XNeg,
				Y,
				YNeg,
				Z,
				ZNeg,
			};

		public:
			IMeshBuffer* Buffer;

			core::vector3df Position[6];

			DECLARE_DATA_TYPE_INDEX;

		protected:
			std::vector<std::pair<int, float>> m_sortPosition;

		public:
			CViewpointData();

			virtual ~CViewpointData();

			void updateBillboard(const core::vector3df& look, const core::vector3df& up, const core::vector3df& campos);

			EAxis hit(CCamera* camera, float x, float y, int viewportW, int viewportH);
		};
	}
}