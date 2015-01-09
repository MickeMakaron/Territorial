template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, Pair()));

    if(!inserted.second)
    {
        //If insertion failed (i.e., ID is already loaded), increment the existing ID's counter
        inserted.first->second.second++;
    }
    else
    {
        std::shared_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        //If insertion succeeded, move the resource to it and set its counter to 1
        inserted.first->second.first = std::move(resource);
        inserted.first->second.second = 1;
    }
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const Resource& resource)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, Pair()));

    if(!inserted.second)
    {
        //If insertion failed (i.e., ID is already loaded), increment the existing ID's counter
        inserted.first->second.second++;
    }
    else
    {
        //If insertion succeeded, move the resource to it and set its counter to 1
        inserted.first->second.first = std::make_shared<Resource>(resource);
        inserted.first->second.second = 1;
    }
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, Pair()));

    if(!inserted.second)
    {
        //If insertion failed (i.e., ID is already loaded), increment the existing ID's counter
        inserted.first->second.second++;
    }
    else
    {
        std::shared_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename, secondParam))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        //If insertion succeeded, move the resource to it and set its counter to 1
        inserted.first->second.first = std::move(resource);
        inserted.first->second.second = 1;
    }
}


template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second.first;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second.first;
}

template <typename Resource, typename Identifier>
bool ResourceHolder<Resource, Identifier>::isLoaded(Identifier id)
{
    auto found = mResourceMap.find(id);

    return found != mResourceMap.end();
}


template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::shared_ptr<Resource> resource)
{
	// Insert and check success
	auto inserted = mResourceMap.insert(std::make_pair(id, std::make_pair(std::move(resource), 1)));
	assert(inserted.second);



}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::release(Identifier id)
{
    auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());


    if(found->second.second > 1)
        found->second.second--;
    else
    {
        mResourceMap.erase(found);
    }

}
