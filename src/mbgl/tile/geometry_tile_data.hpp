#pragma once

#include <mbgl/tile/tile_data.hpp>
#include <mbgl/tile/tile_worker.hpp>
#include <mbgl/text/placement_config.hpp>
#include <mbgl/util/feature.hpp>

#include <atomic>
#include <memory>
#include <unordered_map>

namespace mbgl {

class Style;
class AsyncRequest;
class GeometryTile;
class GeometryTileSource;
class FeatureIndex;

class GeometryTileData : public TileData {
public:
    GeometryTileData(const OverscaledTileID&,
                     std::string sourceID,
                     Style&,
                     const MapMode);

    ~GeometryTileData();

    void setData(std::exception_ptr err,
                 std::unique_ptr<GeometryTile> tile,
                 optional<Timestamp> modified_,
                 optional<Timestamp> expires_);

    Bucket* getBucket(const StyleLayer&) override;

    bool parsePending() override;

    void redoPlacement(PlacementConfig config) override;
    void redoPlacement() override;

    void queryRenderedFeatures(
            std::unordered_map<std::string, std::vector<Feature>>& result,
            const GeometryCoordinates& queryGeometry,
            const TransformState&,
            const optional<std::vector<std::string>>& layerIDs) override;

    void cancel() override;

private:
    Style& style;
    Worker& worker;
    TileWorker tileWorker;

    std::unique_ptr<AsyncRequest> workRequest;

    // Contains all the Bucket objects for the tile. Buckets are render
    // objects and they get added by tile parsing operations.
    std::unordered_map<std::string, std::unique_ptr<Bucket>> buckets;

    std::unique_ptr<FeatureIndex> featureIndex;
    std::unique_ptr<const GeometryTile> geometryTile;

    // Stores the placement configuration of the text that is currently placed on the screen.
    PlacementConfig placedConfig;

    // Stores the placement configuration of how the text should be placed. This isn't necessarily
    // the one that is being displayed.
    PlacementConfig targetConfig;

    // Used to signal the worker that it should abandon parsing this tile as soon as possible.
    std::atomic<bool> obsolete { false };
};

} // namespace mbgl