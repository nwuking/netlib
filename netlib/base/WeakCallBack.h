#ifndef WEAKCALLBACK_H_
#define WEAKCALLBACK_H_

#include <functional>
#include <memory>

namespace netlib
{

template<typename CLASS, typename... ARGS>
class WeakCallBack
{
public:
    WeakCallBack(const std::weak_ptr<CLASS> &object,
                const std::function<void(CLASS*, ARGS...)> &function)
        : _object(object), _function(function)
    {
    } 

    void operator()(ARGS&&... args) const {
        std::shared_ptr<CLASS> ptr(_object.lock());
        if(ptr) {
            _function(ptr.get(), std::forward<ARGS>(args)...);
        }
    }

private:
    std::weak_ptr<CLASS> _object;
    std::function<void(CLASS*, ARGS...)> _function;
};

template<typename CLASS, typename... ARGS>
WeakCallBack<CLASS, ARGS...> makeWeakCallBack(const std::shared_ptr<CLASS> &object,
                                              void (CLASS::*function)(ARGS...))
{
    return WeakCallBack<CLASS, ARGS...>(object, function);
}

template<typename CLASS, typename... ARGS>
WeakCallBack<CLASS, ARGS...> makeWeakCallBack(const std::shared_ptr<CLASS> &object,
                                              void (CLASS::*function)(ARGS...) const)
{
    return WeakCallBack<CLASS, ARGS...>(object, function);
}

}

#endif