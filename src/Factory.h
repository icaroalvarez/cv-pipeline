#pragma once

#include <memory>
#include <map>
#include <functional>

template <class TProduct>
class Factory {
public:

    /**
     * @brief maker products (smart pointer objects) already registered
     * @param name product name
     * @return product (smart pointer to an object)
     */
    std::unique_ptr<TProduct> make(const std::string& name)
    {
        const auto iterator = makers.find(name);
        if(iterator == makers.end())
        {
            throw std::invalid_argument(name+" not registered");
        }
        return makers[name]();
    };

    /**
     * @brief register a maker
     * @tparam TMakerType maker type
     * @param name maker name
     */
    template <typename TMakerType>
    void registerMaker(const std::string& name){ makers[name] = []{return std::make_unique<TMakerType>();}; };
private:
    std::map<std::string, std::function<std::unique_ptr<TProduct>()>> makers;
};
