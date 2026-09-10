# Upgrading from Guichan 0.8 to 0.9

Guichan 0.9 is **neither source nor binary compatible** with 0.8. The soname was
raised to `0.9`, so both versions can be installed side by side and a project can
be ported at its own pace.

This document lists the public API changes, in roughly the order you are likely
to trip over them.

## 1. `BasicContainer` is gone

`gcn::BasicContainer` has been merged into `gcn::Widget`. Every widget can now
have children, which removes the split between "widget" and "container widget".

* Delete `#include <guichan/basiccontainer.hpp>`; the header no longer exists.
* Replace `public BasicContainer` with `public Widget` in your class
  declarations.
* `add()`, `remove()`, `clear()`, `findWidgetById()`, `moveToTop()`,
  `moveToBottom()`, `focusNext()`, `focusPrevious()`, `showWidgetPart()` and
  `getChildren()` now live on `Widget`.
* The child list member `mWidgets` is now called `mChildren`. The static list of
  every live widget, previously also called `mWidgets`, is now
  `mWidgetInstances`.
* `Widget::getChildren()` is protected; `Container::getChildren()` is public.

### `logicChildren()` was removed, `drawChildren()` is called for you

Children are now updated automatically by `Widget::_logic()`, which the parent
calls for you, so `logicChildren()` is gone. `drawChildren(graphics)` still
exists as a protected virtual, but `Widget::_draw()` now calls it after
`draw()` rather than leaving it to `draw()` to call. Remove every explicit call
to `drawChildren(graphics)` and `logicChildren()`:

```c++
// 0.8
void MyWidget::draw(gcn::Graphics *graphics)
{
    drawBackground(graphics);
    drawChildren(graphics);
}

// 0.9
void MyWidget::draw(gcn::Graphics *graphics)
{
    drawBackground(graphics);
}
```

If you need to wrap child drawing (for example to apply an extra clip
rectangle), override `drawChildren()` and call the base implementation, which
pushes the children area as clip area and draws the visible children:

```c++
void MyScrollArea::drawChildren(gcn::Graphics *graphics)
{
    graphics->pushClipArea(getChildrenArea());
    gcn::Widget::drawChildren(graphics);
    graphics->popClipArea();
}
```

### `getChildrenArea()` still decides what is drawn

`Widget::getChildrenArea()` returns the widget's whole area by default, the same
as `BasicContainer::getChildrenArea()` did in 0.8, so a straight port of a
container widget needs no change here.

It is worth knowing what the method now controls, because it is easy to break by
accident. Children outside the children area are neither drawn nor hit-tested, so
a widget whose `getChildrenArea()` returns an empty rectangle appears to have no
children at all, with no error and nothing in the output. Override it only to
confine children to a smaller area, for instance to leave room for a frame or a
title bar:

```c++
gcn::Rectangle MyWidget::getChildrenArea()
{
    return gcn::Rectangle(1, mTitleBarHeight, getWidth() - 2,
                          getHeight() - mTitleBarHeight - 1);
}
```

`gcn::Container`, `gcn::Window`, `gcn::ScrollArea` and `gcn::DropDown` override
it for exactly that reason.

### Child lifetime

Widgets no longer register a death listener on each child. Instead
`Widget::~Widget()` removes itself from its parent and detaches its own
children. If you relied on `BasicContainer::death()` to clean up, that hook is
gone.

Note that `~Widget()` detaches children but does not reset their focus handler.
If you destroy a parent that installed an internal focus handler while its
children outlive it, clear the children out yourself first.

## 2. Enum constants were renamed

All enum constants were renamed from `SHOUTING_CASE` to `CamelCase` so they stop
colliding with macros from other libraries (`windows.h` in particular). As a
side effect, `key.hpp` no longer has to `#undef DELETE` on Windows.

| 0.8 | 0.9 |
| --- | --- |
| `Key::SPACE`, `TAB`, `ENTER` | `Key::Space`, `Tab`, `Enter` |
| `Key::LEFT_ALT`, `RIGHT_ALT`, `ALT_GR` | `Key::LeftAlt`, `RightAlt`, `AltGr` |
| `Key::LEFT_SHIFT`, `LEFT_CONTROL`, `LEFT_META`, `LEFT_SUPER` (and `RIGHT_*`) | `Key::LeftShift`, `LeftControl`, `LeftMeta`, `LeftSuper` (and `Right*`) |
| `Key::INSERT`, `HOME`, `END`, `DELETE` | `Key::Insert`, `Home`, `End`, `Delete` |
| `Key::PAGE_UP`, `PAGE_DOWN` | `Key::PageUp`, `PageDown` |
| `Key::ESCAPE`, `BACKSPACE`, `PAUSE` | `Key::Escape`, `Backspace`, `Pause` |
| `Key::CAPS_LOCK`, `NUM_LOCK`, `SCROLL_LOCK`, `PRINT_SCREEN` | `Key::CapsLock`, `NumLock`, `ScrollLock`, `PrintScreen` |
| `Key::LEFT`, `RIGHT`, `UP`, `DOWN` | `Key::Left`, `Right`, `Up`, `Down` |
| `Graphics::LEFT`, `CENTER`, `RIGHT` | `Graphics::Left`, `Center`, `Right` |
| `MouseInput::MOVED`, `PRESSED`, `RELEASED` | `MouseInput::Moved`, `Pressed`, `Released` |
| `MouseInput::WHEEL_MOVED_UP`, `WHEEL_MOVED_DOWN` | `MouseInput::WheelMovedUp`, `WheelMovedDown` |
| `MouseInput::EMPTY`, `LEFT`, `RIGHT`, `MIDDLE` | `MouseInput::Empty`, `Left`, `Right`, `Middle` |
| `MouseEvent::CLICKED`, `ENTERED`, `EXITED`, `DRAGGED` | `MouseEvent::Clicked`, `Entered`, `Exited`, `Dragged` |
| `KeyInput::PRESSED`, `RELEASED` | `KeyInput::Pressed`, `Released` |
| `KeyEvent::PRESSED`, `RELEASED` | `KeyEvent::Pressed`, `Released` |
| `ScrollArea::SHOW_ALWAYS`, `SHOW_NEVER`, `SHOW_AUTO` | `ScrollArea::ShowAlways`, `ShowNever`, `ShowAuto` |
| `Slider::HORIZONTAL`, `VERTICAL` | `Slider::Horizontal`, `Vertical` |

The function key constants `Key::F1` through `Key::F15` are unchanged.

### Key values changed

The non-character key constants moved from `1000, 1001, ...` to
`-1000, -999, ...`, so they can no longer collide with Unicode code points above
1000 coming out of a backend. `Key::getValue()` still returns `int`.

If you store Guichan key values anywhere persistent (a keybinding config file),
or in an `unsigned` variable, that code needs to change. Storing backend-native
key codes (`SDLK_*` and friends) instead avoids the problem entirely.

## 3. Const correctness

These became `const`. Any override in your code has to match, or it silently
stops overriding.

| 0.8 | 0.9 |
| --- | --- |
| `Widget::getId()` | `Widget::getId() const` |
| `ListBox::getListModel()` | `ListBox::getListModel() const` |
| `DropDown::getListModel()` | `DropDown::getListModel() const` |
| `ScrollArea::getContent()` | `ScrollArea::getContent() const` |
| `Window::isOpaque()` | `Window::isOpaque() const` |
| `Window::getTitleBarHeight()` | `Window::getTitleBarHeight() const` |
| `TabbedArea::isTabSelected(Tab *)` | `TabbedArea::isTabSelected(Tab *) const` |
| `TabbedArea::getSelectedTab()` | `TabbedArea::getSelectedTab() const` |

## 4. Other signature changes

* `Rectangle::isPointInRect(int, int)` is now `Rectangle::isContaining(int, int)`.
* `TextField::getText()` and `TextBox::getTextRow(int)` return `std::string` by
  value instead of `const std::string &`. Binding the result to a
  `const std::string &` still works; taking its address or holding the reference
  across a mutation does not.
* `TextBox::addRow(const std::string)` is now
  `TextBox::addRow(const std::string &)`.
* `TextField::fontChanged()` was removed.
* `TabbedArea::logic()` was removed; `Widget::_logic()` handles the tabs and the
  active widget.
* `InputEvent`, `MouseEvent` and `KeyEvent` constructors take an extra
  `Widget *distributor` after the source widget. This only matters if you
  construct events yourself, for example in a custom `Gui` or input backend.

## 5. `TextField` and `TextBox` now use `gcn::Text`

Text handling in both widgets moved into a new helper class, `gcn::Text`
(`<guichan/text.hpp>`). The protected members it replaced are gone:

* `TextField::mText` is now a `Text *`, not a `std::string`.
* `TextField::mCaretPosition` was removed. Use `getCaretPosition()` and
  `setCaretPosition()`.
* `TextBox::mTextRows`, `TextBox::mCaretRow` and `TextBox::mCaretColumn` were
  removed. Use `getNumberOfRows()`, `getTextRow()`, `setTextRow()`,
  `getCaretRow()` and `getCaretColumn()`.

Subclasses that poked at those members to implement custom editing or drawing
need reworking against the public interface.

## 6. Listener typedefs removed

`Widget::MouseListenerList`, `MouseListenerIterator`, `KeyListenerList`,
`ActionListenerList`, `DeathListenerList`, `FocusListenerList`,
`WidgetListenerList` and their iterator typedefs are gone. The members are now
plain `std::list<T *>`; use `std::list<gcn::MouseListener *>::iterator`, or
`auto`.

## 7. Headers no longer include transitively

Several headers replaced includes with forward declarations. `dropdown.hpp` in
particular no longer pulls in `listbox.hpp`, `scrollarea.hpp`, `listmodel.hpp`
or `deathlistener.hpp`. If you relied on those arriving indirectly, include them
yourself.

## 8. Graphics

`Graphics` gained inline `int` overloads:

```c++
void drawRectangle(int x, int y, int width, int height);
void fillRectangle(int x, int y, int width, int height);
```

Because they are overloads of existing names, a subclass that declares only
`drawRectangle(const Rectangle &)` **hides** the `int` version. Pull it back in:

```c++
class MyGraphics : public gcn::Graphics
{
public:
    using gcn::Graphics::drawRectangle;
    using gcn::Graphics::fillRectangle;

    void drawRectangle(const gcn::Rectangle &rectangle) override;
    void fillRectangle(const gcn::Rectangle &rectangle) override;
};
```

`SDLImage::convertToDisplayFormat()` now throws a `gcn::Exception` when the
conversion fails, instead of failing silently.

## 9. Mouse entered and exited event order

Mouse entered and exited events are now delivered in widget hierarchy order:
entered events outermost widget first, exited events innermost widget first. In
0.8 entered events went the other way, innermost first.

Which widgets receive the events is unchanged from 0.8, including a widget that
is hidden, removed or moved out from under the mouse still being exited.

## 10. Additions worth adopting

* `gcn::ContainerListener` and `gcn::ContainerEvent`, for `widgetAdded` and
  `widgetRemoved` notifications from a `Container`.
* `Widget::getTop()`, the root of the widget's parent chain.
* `Widget::getWidgetsIn(const Rectangle &, Widget *ignore = NULL)`.
* `Widget::resizeToChildren()` and `Container::resizeToContent()`.
* `Widget::isSetVisible()`, the widget's own visibility flag, as opposed to
  `isVisible()` which also considers the parents.
* `Rectangle::intersection()`, `Rectangle::isContaining(const Rectangle &)`,
  `Rectangle::isEmpty()`, `operator+` and `operator+=`.
* `TabbedArea::getNumberOfTabs()` and `TabbedArea::setBaseColor()`.
* `TextField::setEditable()` and `isEditable()`, for a read-only text field.
* `InputEvent::getDistributor()`, the widget that distributed the event, as
  opposed to `getSource()`.
* `Gui::getWidgetsAt(int, int)`, returning the widgets under a coordinate as a
  `std::vector<Widget *>` ordered from the top widget down to the deepest one.

## 11. Build and packaging

* The soname is `0.9`, so the shared library is `libguichan-0.9.so`.
* Only an unversioned `guichan.pc` is installed, reporting version `0.9.0`.
  0.8 also installed `guichan-0.8.pc`; there is no `guichan-0.9.pc`
  equivalent, so a `pkg-config guichan-0.8` check has to become
  `pkg-config --atleast-version=0.9.0 guichan`.
* CMake gained an optional Irrlicht extension (`-DENABLE_IRRLICHT=ON`).

## Porting checklist

1. Drop `basiccontainer.hpp` includes, change `BasicContainer` bases to `Widget`.
2. Remove all `drawChildren()` and `logicChildren()` calls; `_draw()` now calls
   `drawChildren()` for you. To wrap child drawing, override `drawChildren()`
   and call the base implementation.
3. Keep any `getChildrenArea()` override that confines children to a smaller
   area; the full-area case is now the default and can be dropped.
4. Rename `mWidgets` to `mChildren` in your container subclasses.
5. Rename the enum constants (see the table in section 2).
6. Add `const` to the overrides listed in section 3.
7. Rename `isPointInRect` to `isContaining`.
8. Rework any `TextField` or `TextBox` subclass that touched `mText`,
   `mCaretPosition`, `mTextRows`, `mCaretRow` or `mCaretColumn`.
9. Add `using` declarations in `Graphics` subclasses for the new `int` overloads.
10. Build with warnings enabled and mark every intended override with
    `override`; that catches the accidentally-hidden virtuals from sections 3
    and 8 at compile time.
