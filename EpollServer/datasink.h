#pragma once

class DataSink
{
public:
    virtual ~DataSink(){}
    virtual void onDataIndication(const char *buf, const int nbytes) = 0;
};