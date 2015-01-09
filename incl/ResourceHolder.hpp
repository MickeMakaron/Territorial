#ifndef ANTGAME_RESOURCEHOLDER_HPP
#define ANTGAME_RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>






template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void                load(Identifier id,const std::string& filename);
        void                load(Identifier id, const Resource& resource);


        template <typename Parameter>
        void                load(Identifier id, const std::string& filename, const Parameter& secondParam);

        Resource&           get(Identifier id);
        const Resource&     get(Identifier id) const;

        bool                isLoaded(Identifier id);

        void  release(Identifier id);

    private:
        typedef std::pair<std::shared_ptr<Resource>, int> Pair;
        void insertResource(Identifier id, std::shared_ptr<Resource> resource);

    private:
        std::map<Identifier, Pair > mResourceMap;
};

#include "ResourceHolder.inl"

#endif // ANTGAME_RESOURCEHOLDER_HPP
