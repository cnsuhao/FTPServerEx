#ifndef __H_NOCOPYABLE__
#define __H_NOCOPYABLE__

class NoCopyable
{
public:
    NoCopyable() {}
    virtual ~NoCopyable() {}

private:
    NoCopyable(const NoCopyable &) {}
    NoCopyable &operator=(const NoCopyable &) {return *this;}
};

#endif // __H_NOCOPYABLE__
