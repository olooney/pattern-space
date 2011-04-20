
    // interface used to communicate to user input to a Solid
    class Controls {
    public:
        virtual void up(bool) = 0;
        virtual void down(bool) = 0;
        virtual void left(bool) = 0;
        virtual void right(bool) = 0;
        virtual void primary(bool) = 0;
        virtual void secondary(bool) = 0;
        virtual void tertiary(bool) = 0;
    }; // end class Controls

