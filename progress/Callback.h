#pragma once

#include <string>

enum class Severity : unsigned short {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Critical = 4,
    Progress = 5
};

class Callback {
public:
    virtual ~Callback() = default;
    virtual void notify( Severity s, const std::string& m );
    virtual void progress( unsigned short percent );
    virtual bool isCancelled() const;
};

class PrintCallback : public Callback {
public:
    void notify( Severity s, const std::string& m ) override;
    void progress( unsigned short percent ) override;
};
