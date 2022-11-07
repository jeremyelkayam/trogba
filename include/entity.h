#pragma once


namespace trog{

    class entity{ 
    public:
        virtual ~entity() = default;
        virtual void update() = 0;
    protected:
        entity() = default;
    };

}