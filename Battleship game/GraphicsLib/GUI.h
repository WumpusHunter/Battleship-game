//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

//------------------------------------------------------------------------------

#ifndef GUI_GUARD
#define GUI_GUARD

//------------------------------------------------------------------------------

#include "Window.h"
#include "Graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

    //------------------------------------------------------------------------------

    // FLTK's required function type for all callback functions
    typedef void* Address;
    typedef void(*Callback)(Address, Address);

    //------------------------------------------------------------------------------

    // Gets reference to content of pw, assuming that W is it's type
    template<class W> W& reference_to(Address pw)
    {
        if (pw == nullptr)      // Handle of NULL pointer
            throw std::invalid_argument("Bad reference_to: address of NULL");
        return *static_cast<W*>(pw);
    }

    //------------------------------------------------------------------------------

    // Invariant: width >= 0, height >= 0
    // Note: to access widget, first connect it to window
    class Widget {      // Handle for Fl_Widget
    public:
        // Constructs widget with top-left angle at xy, of size w * h, labeled lab, and cb as callback function
        Widget(Point xy, unsigned int w, unsigned int h, const std::string& lab, Callback cb)
            : loc{ xy }, width{ w }, height{ h }, label{ lab }, do_it{ cb }, pw{ nullptr }, own{ nullptr } {}

        // Destructs widget
        virtual ~Widget() { }

        // Access to parameters (writing)
        virtual void move(int dx,int dy) { hide(); pw->position(loc.x+=dx, loc.y+=dy); show(); }
        virtual void hide() { pw->hide(); }
        virtual void show() { pw->show(); }
        virtual void activate() { pw->activate(); }
        virtual void deactivate() { pw->deactivate(); }

        // Access to parameters (reading)
        virtual unsigned int visible() const { return pw->visible(); }
        virtual unsigned int active() const { return pw->active(); }
        virtual const Address widget() const { return pw; }

        // Connection with window
        virtual void attach(Window&) = 0;

        // Forbidden copying
        Widget& operator=(const Widget&) = delete;
        Widget(const Widget&) = delete;

        Point loc;              // Top-left angle
        unsigned int width;
        unsigned int height;
        std::string label;
        Callback do_it;         // Callback function

    protected:
        Window* own;        // Address of widget's owner
        Fl_Widget* pw;      // Address of FLTK's widget
    };

    //------------------------------------------------------------------------------

    // Invariant: width >= 0, height >= 0
    struct Button : Widget {    // Handle for Fl_Button
    public:
        // Constructs button with top-left angle at xy, of size
        // w * h, labeled lab, and cb as callback function
        Button(Point xy, unsigned int w, unsigned int h, const std::string& lab, Callback cb)
            : Widget{ xy, w, h, lab, cb } {}

        // Connection with window
        void attach(Window&);
    };

    //------------------------------------------------------------------------------

    // Invariant: width >= 0, height >= 0
    struct In_box : Widget {    // Handle for Fl_Input
    public:
        // Constructs input box with top-left angle at xy, of size w * h, labeled lab
        In_box(Point xy, unsigned int w, unsigned int h, const std::string& lab)
            : Widget{ xy, w, h, lab, nullptr } { }

        // Access to parameters (reading)
        int get_int() const;
        std::string get_string() const;

        // Connection with window
        void attach(Window&);
    };

    //------------------------------------------------------------------------------

    // Invariant: width >= 0, height >= 0
    struct Out_box : Widget {   // Handle for Fl_Output
    public:
        // Constructs output box with top-left angle at xy, of size w * h, labeled lab
        Out_box(Point xy, unsigned int w, unsigned int h, const std::string& lab)
            : Widget{ xy, w, h, lab, nullptr } { }

        // Access to parameters (writing)
        void put(int);
        void put(const std::string&);

        // Access to parameters (reading)
        int get_int() const;
        std::string get_string() const;

        // Connection with window
        void attach(Window&);
    };

    //------------------------------------------------------------------------------

    // Invariant: width >= 0, height >= 0
    struct Menu : Widget {
    public:
        enum class Kind { horizontal, vertical };

        // Constructs menu with top-left angle at xy, of w * h size for each button, oriented with k, labeled lab
        Menu(Point xy, unsigned int w, unsigned int h, Kind k, const std::string& lab)
            : Widget{ xy, w, h, lab, nullptr }, kind{ k }, offset{ 0 } {}

        // Access to parameters (writing)
        void show()
        {
            // Show all buttons
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].show();
        }
        void hide()
        {
            // Hide all buttons
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].hide();
        }
        void move(int dx, int dy)
        {
            // Move all buttons
            for (unsigned int i = 0; i < selection.size(); ++i) 
                selection[i].move(dx,dy);
        }

        // Connection with buttons
        unsigned int attach(Button& but);      // but isn't owned by menu
        unsigned int attach(Button* but);      // but is owned by menu

        // Connection with window
        void attach(Window& win)
        {
            // Attach all buttons to win
            for (unsigned int i = 0; i < selection.size(); ++i)
                win.attach(selection[i]);
            own = &win;     // Connection with window
        }

        Vector_ref<Button> selection;   // Set of buttons
        Kind kind;                      // Direction of menu
        unsigned int offset;            // Offset between current button and menu

    private:
        // Helper function
        void init(Button& but);          // Initial parameters of but
    };

    //------------------------------------------------------------------------------

    // Invariant: w >= 0, h >= 0
    class Text_box : public Widget {
    public:
        // Construction
        Text_box(Point xy, unsigned int w, unsigned int h, const std::string& lab);

        // Access to parameters (writing)
        void put(const std::string& str);

        // Access to parameters (reading)
        std::string get() const;

        // Connection with window
        void attach(Window&) override;
    };

    //------------------------------------------------------------------------------

    // Invariant: w >= 0, h >= 0
    class Group : public Widget {
    public:
        // Construction
        Group(Point xy, unsigned int but_w, unsigned int but_h,
            unsigned int h_num, unsigned int v_num, const std::string& lab, Callback cb);

        // Access to parameters (writing)
        // Moves all buttons of group dx by x-coordinate and dy by y-coordinate
        void move(int dx, int dy)
        {
            Widget::move(dx, dy);       // Update location of widget
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].move(dx, dy);
        }
        // Shows all buttons of group
        void show()
        {
            //Widget::show();             // Update visibility of widget
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].show();
        }
        // Hides all buttons of group
        void hide()
        {
            //Widget::hide();             // Update visibility of widget
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].hide();
        }
        // Activates all buttons of group
        void activate()
        {
            //Widget::activate();         // Update activity of widget
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].activate();
        }
        // Deactivates all buttons of group
        void deactivate()
        {
            //Widget::deactivate();       // Update activity of widget
            for (unsigned int i = 0; i < selection.size(); ++i)
                selection[i].deactivate();
        }

        // Connection with window
        void attach(Window& win)
        {
            // Attach all buttons to win
            for (unsigned int i = 0; i < selection.size(); ++i)
                win.attach(selection[i]);
            own = &win;     // Connection with window
        }

        Vector_ref<Button> selection;   // Set of buttons
    };

    //------------------------------------------------------------------------------

}   // of namespace Graph_lib

#endif // GUI_GUARD

//------------------------------------------------------------------------------