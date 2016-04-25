#include <mbgl/style/layer.hpp>
#include <mbgl/layer/layer_impl.hpp>

namespace mbgl {

Layer::Layer(Type type_, std::unique_ptr<Impl> baseImpl_)
    : baseImpl(std::move(baseImpl_)), type(type_) {
}

Layer::~Layer() = default;

const std::string& Layer::getID() const {
    return baseImpl->id;
}

VisibilityType Layer::getVisibility() const {
    return baseImpl->visibility;
}

void Layer::setVisibility(VisibilityType value) {
    baseImpl->visibility = value;
}

std::unique_ptr<Layer> Layer::copy(const std::string& id,
                                   const std::string& ref) const {
    std::unique_ptr<Layer> result = clone();
    result->baseImpl->id = id;
    result->baseImpl->ref = ref;
    return result;
}

} // namespace mbgl