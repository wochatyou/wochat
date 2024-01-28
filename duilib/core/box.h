#ifndef UI_CORE_BOX_H_
#define UI_CORE_BOX_H_

namespace ui
{
    class Box;
    class Layout
    {
    public:
        Layout();
        Layout& operator=(const Layout& r) = delete;
        virtual ~Layout() {}

        void SetOwner(Box* pOwner);

    };
}

#endif // UI_CORE_BOX_H_