/*
!@
MIT License

Copyright (c) 2020 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
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

#include <functional>
#include <set>

#include "GUI/Type.h"
#include "GUI/Theme/CTheme.h"
#include "GUI/Renderer/CRenderer.h"

#define BIND_LISTENER(A, B) std::bind(A, B, std::placeholders::_1)

namespace Skylicht
{
	namespace Editor
	{
		namespace GUI
		{
			class CCanvas;
			class CBase;

			struct SDragDropPackage
			{
				std::string Name;
				CBase* Control;
				CBase* DrawControl;
				void* UserData;
				float HoldOffsetX;
				float HoldOffsetY;

				SDragDropPackage()
				{
					Control = NULL;
					UserData = NULL;
					DrawControl = NULL;
					HoldOffsetX = 0.0f;
					HoldOffsetY = 0.0f;
				}
			};

			class CBase
			{
			public:
				typedef std::list<CBase*> List;
				typedef std::function<void(CBase*)> Listener;
				typedef std::map<std::string, Listener> AccelMap;

				CBase(CBase* parent, const std::string& name = "");
				virtual ~CBase();

			public:
				virtual void remove();

				virtual void setHidden(bool hidden);

				//!< Returns true only if this control is hidden.
				virtual bool isHidden() const;

				//!< Returns false if this control or its parents are hidden.
				virtual bool isVisible() const;

				virtual void setDisabled(bool disable);

				virtual bool isDisabled()
				{
					return m_disabled;
				}

				const std::string& getName()
				{
					return m_name;
				}

			protected:

				virtual void addChild(CBase* child);
				virtual void removeChild(CBase* child);
				virtual void onChildAdded(CBase* child)
				{
					invalidate();
				}

				virtual void onChildRemoved(CBase* child)
				{
					invalidate();
				}

				virtual void onBoundsChanged(const SRect& oldBounds);
				virtual void onChildBoundsChanged(const SRect& oldChildBounds, CBase* child);

			public:
				List Children;

				virtual void removeAllChildren();
				virtual void releaseChildren();
				virtual void sendToBack(void);
				virtual void bringToFront(void);
				virtual void bringNextToControl(CBase* child, bool behind);
				virtual void bringSwapChildControl(CBase* control1, CBase* control2);

				virtual CBase* findChildByName(const std::string& name, bool recursive);

				virtual bool isChild(CBase* child, bool recursive);

				virtual u32 numChildren();
				virtual CBase* getChild(u32 i);

				virtual void setParent(CBase* parent);

				virtual CBase* getParent()
				{
					return m_parent;
				}

				virtual CBase* getInnerPanel()
				{
					if (m_innerPanel == NULL)
						return this;

					return m_innerPanel;
				}

				virtual const List& getChildren()
				{
					return Children;
				}

				virtual CCanvas* getCanvas();

			public:

				virtual void invalidate()
				{
					m_needsLayout = true;
				}

				void invalidateParent()
				{
					if (m_parent)
						m_parent->invalidate();
				}

				void invalidateChildren();

			public:

				inline void setDebugValue(int d)
				{
					m_debugValue = d;
				}

				inline void tagInt(int intValue)
				{
					m_tagInt = intValue;
				}

				inline int getTagInt()
				{
					return m_tagInt;
				}

				inline void tagFloat(float floatValue)
				{
					m_tagFloat = floatValue;
				}

				inline float getTagFloat()
				{
					return m_tagFloat;
				}

				inline void tagBool(bool boolValue)
				{
					m_tagBool = boolValue;
				}

				inline bool getTagBool()
				{
					return m_tagBool;
				}

				inline void tagString(const std::string& s)
				{
					m_tagString = s;
				}

				inline const std::string& getTagString()
				{
					return m_tagString;
				}

				inline void tagString(const std::wstring& s)
				{
					m_tagWString = s;
				}

				inline const std::wstring& getTagWString()
				{
					return m_tagWString;
				}

				inline void tagData(void* data)
				{
					m_tagData = data;
				}

				inline void* getTagData()
				{
					return m_tagData;
				}

				inline void setTabable(bool b)
				{
					m_isTabable = b;
				}

				inline bool isTabable()
				{
					return m_isTabable;
				}

				inline float X() const { return m_bounds.X; }
				inline float Y() const { return m_bounds.Y; }

				inline float width() const { return m_bounds.Width; }
				inline float height() const { return m_bounds.Height; }

				virtual float bottom() const
				{
					return m_bounds.Y + m_bounds.Height + m_margin.Bottom;
				}

				virtual float right() const
				{
					return m_bounds.X + m_bounds.Width + m_margin.Right;
				}

				inline const SMargin& getMargin() const { return m_margin; }
				inline const SPadding& getPadding() const { return m_padding; }

				inline void setPos(float x, float y) { setBounds(x, y, width(), height()); }
				inline void setPos(const SPoint& p) { return setPos(p.X, p.Y); }
				inline SPoint getPos() const { return SPoint(X(), Y()); }

				void setCenterPosition();

				inline void setWidth(float w) { setSize(w, height()); }
				inline void setHeight(float h) { setSize(width(), h); }

				inline bool setSize(float w, float h) { return setBounds(X(), Y(), w, h); }
				inline bool setSize(const SDimension& p) { return setSize(p.Width, p.Height); }
				inline SDimension getSize() const { return SDimension(width(), height()); }

				SDimension getChildrenSize();
				void sizeToChildren(bool w, bool h);

				virtual void sizeToContents() { }

				inline bool setBounds(float x, float y, float w, float h) { return setBounds(SRect(x, y, w, h)); }
				virtual bool setBounds(const SRect& bounds);

				inline void setPadding(const SPadding& padding) { m_padding = padding; }
				inline void setMargin(const SMargin& margin) { m_margin = margin; }

				virtual void moveTo(float x, float y);
				virtual void dragTo(float x, float y, float dragPosX, float dragPosY, float paddingBottom, bool clampDragPos);
				inline void moveBy(float x, float y) { moveTo(X() + x, Y() + y); }

				inline const SRect& getBounds() const { return m_bounds; }
				inline const SRect& getRenderBounds() const { return m_renderBounds; }
				inline const SRect& getInnerBounds() const { return m_innerBounds; }

				inline EPosition getDock() { return m_dock; }
				void dock(EPosition dock);

				virtual void updateRenderBounds();

			public:

				virtual void think() {}

				virtual void doRender();

				virtual void doRenderDragOverlay();

				virtual void doRenderOverlay();

				virtual void renderRecursive(const SRect& cliprect);

				virtual void renderOverlay();

				bool shouldClip() { return m_shouldClip; }

				inline void enableClip(bool b)
				{
					m_shouldClip = b;
				}

				inline void enableRenderFillRect(bool b)
				{
					m_renderFillRect = b;
				}

				inline void setFillRectColor(const SGUIColor& c)
				{
					m_fillRectColor = c;
				}

			protected:

				virtual void render();
				virtual void renderFocus();
				virtual void renderUnder() {}
				virtual void renderOver() {}

			public:
				inline void setCursor(ECursorType cursor)
				{
					m_cursor = cursor;
				}

				virtual void updateCursor();

				virtual void onMouseMoved(float x, float y, float deltaX, float deltaY)
				{
					if (OnMouseMoved != nullptr)
						OnMouseMoved(this, x, y, deltaX, deltaY);
				}

				virtual bool onMouseWheeled(int delta);

				virtual void onMouseClickLeft(float x, float y, bool down)
				{
					if (OnLeftMouseClick != nullptr)
						OnLeftMouseClick(this, x, y, down);
				}

				virtual void onMouseClickRight(float x, float y, bool down)
				{
					if (OnRightMouseClick != nullptr)
						OnRightMouseClick(this, x, y, down);
				}

				virtual void onMouseClickMiddle(float x, float y, bool down)
				{
					if (OnMiddleMouseClick != nullptr)
						OnMiddleMouseClick(this, x, y, down);
				}

				virtual void onMouseDoubleClickLeft(float x, float y)
				{
					onMouseClickLeft(x, y, true);
					if (OnDoubleLeftMouseClick != nullptr)
						OnDoubleLeftMouseClick(this, x, y);
				}

				virtual void onMouseTripleClickLeft(float x, float y)
				{
					onMouseClickLeft(x, y, true);
				}

				virtual void onMouseDoubleClickRight(float x, float y)
				{
					onMouseClickRight(x, y, true);
					if (OnDoubleRightMouseClick != nullptr)
						OnDoubleRightMouseClick(this, x, y);
				}

				virtual void onMouseTripleClickRight(float x, float y)
				{
					onMouseClickRight(x, y, true);
				}

				virtual void onMouseDoubleClickMiddle(float x, float y)
				{
					onMouseClickMiddle(x, y, true);
					if (OnDoubleMiddleMouseClick != nullptr)
						OnDoubleMiddleMouseClick(this, x, y);
				}

				virtual void onMouseTripleClickMiddle(float x, float y)
				{
					onMouseClickMiddle(x, y, true);
				}

				virtual void onMouseEnter();
				virtual void onMouseLeave();

				virtual void onLostKeyboardFocus()
				{
					m_keyboardFocus = false;
					if (OnLostKeyboardFocus != nullptr)
						OnLostKeyboardFocus(this);
				}

				virtual void onKeyboardFocus()
				{
					m_keyboardFocus = true;
					if (OnKeyboardFocus != nullptr)
						OnKeyboardFocus(this);
				}

				inline bool isKeyboardFocus()
				{
					return m_keyboardFocus;
				}

				virtual void setTransparentMouseInput(bool b) { m_transparentMouseInput = b; }
				virtual void setMouseInputEnabled(bool b) { m_mouseInputEnabled = b; }
				virtual bool getMouseInputEnabled() { return m_mouseInputEnabled; }

				virtual void setKeyboardInputEnabled(bool b) { m_keyboardInputEnabled = b; }
				virtual bool getKeyboardInputEnabled() const { return m_keyboardInputEnabled; }

				virtual bool needsInputChars() { return false; }

				virtual bool onChar(u32 c) {
					if (OnChar != nullptr)
						OnChar(this, c);
					return false;
				}

				virtual bool onKeyPress(EKey key, bool press = true);
				virtual bool onKeyRelease(EKey key);

				virtual bool onKeyTab(bool down);
				virtual bool onKeySpace(bool down) { return false; }
				virtual bool onKeyReturn(bool down) { return false; }
				virtual bool onKeyBackspace(bool down) { return false; }
				virtual bool onKeyDelete(bool down) { return false; }
				virtual bool onKeyRight(bool down) { return false; }
				virtual bool onKeyLeft(bool down) { return false; }
				virtual bool onKeyHome(bool down) { return false; }
				virtual bool onKeyEnd(bool down) { return false; }
				virtual bool onKeyUp(bool down) { return false; }
				virtual bool onKeyDown(bool down) { return false; }
				virtual bool onKeyEscape(bool down) { return false; }

				virtual void onBeginMoved() {}
				virtual void onMoved() {}
				virtual void onEndMoved() {}
				virtual void onResized() {}

				virtual bool isHovered() const;
				virtual bool shouldDrawHover() const;

				virtual void touch();
				virtual void onChildTouched(CBase* child);

				virtual bool isOnTop();

				virtual bool isFocussed();
				virtual void focus();
				virtual void unfocus();
				virtual void onTabableFocus() {}

				virtual void onStartDragging(SDragDropPackage* package, int x, int y) {}
				virtual void onDraggingEnter(SDragDropPackage* package, int x, int y) {}
				virtual void onDraggingHover(SDragDropPackage* package, int x, int y) {}
				virtual void onDrop(SDragDropPackage* package, int x, int y) {}

			public:

				virtual CBase* getControlAt(float x, float y, bool onlyIfMouseEnabled = true);
				virtual CBase* getControlAt(float x, float y, const std::type_info& type, bool onlyIfMouseEnabled = true);

				virtual SPoint localPosToCanvas(const SPoint& in = SPoint(0.0f, 0.0f));
				virtual SPoint canvasPosToLocal(const SPoint& in);

				virtual SPoint getMinimumSize()
				{
					return SPoint(35.0f, 35.0f);
				}

				virtual void recurseLayout();
				virtual void layout();
				virtual void postLayout() {}

			public:

				virtual void closeMenu() {}

				virtual bool isMenuComponent();

			public:

				void addAccelerator(const std::string& accelerator, const Listener& function);

				virtual bool handleAccelerator(const std::string& accelerator);

				inline bool accelOnlyFocus()
				{
					return m_accelOnlyFocus;
				}

				SDragDropPackage* setDragDropPackage(const std::string& name, void* userData);

				inline SDragDropPackage* getDragDropPackage()
				{
					return m_dragDropData;
				}

				virtual void onStartDragging(float x, float y);

			public:

				Listener OnHoverEnter;
				Listener OnHoverLeave;
				Listener OnDestroy;
				Listener OnResize;
				Listener OnRender;
				Listener OnLostKeyboardFocus;
				Listener OnKeyboardFocus;

				std::function<bool()> OnShouldDrag;
				std::function<bool(SDragDropPackage*, float, float)> OnStartDragging;
				std::function<bool(SDragDropPackage*)> OnAcceptDragDrop;
				std::function<void(SDragDropPackage*, float, float)> OnDragDropHover;
				std::function<void(SDragDropPackage*, float, float)> OnDragDropOut;
				std::function<void(SDragDropPackage*, float, float)> OnDrop;

				std::function<void(CBase*, float, float, float, float)> OnMouseMoved;
				std::function<void(CBase*, float, float, bool)> OnLeftMouseClick;
				std::function<void(CBase*, float, float, bool)> OnRightMouseClick;
				std::function<void(CBase*, float, float, bool)> OnMiddleMouseClick;
				std::function<void(CBase*, float, float)> OnDoubleLeftMouseClick;
				std::function<void(CBase*, float, float)> OnDoubleRightMouseClick;
				std::function<void(CBase*, float, float)> OnDoubleMiddleMouseClick;
				std::function<void(CBase*, int)> OnMouseWheeled;
				std::function<void(CBase*, int, bool)> OnKeyPress;
				std::function<void(CBase*, u32)> OnChar;

			protected:
				CBase* m_parent;
				CBase* m_innerPanel;

				std::string m_name;

				SRect m_bounds;
				SRect m_renderBounds;
				SRect m_innerBounds;

				SPadding m_padding;
				SMargin m_margin;

				int m_debugValue;

				bool m_disabled;
				bool m_hidden;
				bool m_needsLayout;

				bool m_shouldClip;

				bool m_transparentMouseInput;
				bool m_mouseInputEnabled;
				bool m_keyboardInputEnabled;

				bool m_renderFillRect;
				SGUIColor m_fillRectColor;

				bool m_renderDragDrop;

				EPosition m_dock;

				ECursorType m_cursor;

				int m_tagInt;
				float m_tagFloat;
				bool m_tagBool;
				std::string m_tagString;
				std::wstring m_tagWString;
				void* m_tagData;

				bool m_keyboardFocus;

				AccelMap m_accelerators;
				bool m_accelOnlyFocus;

				bool m_isTabable;

				SDragDropPackage* m_dragDropData;
			};
		}
	}
}