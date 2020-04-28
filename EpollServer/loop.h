#pragma once

class Loop
{
public:
    virtual ~Loop(){}

protected:
    virtual void _Run() = 0;
};