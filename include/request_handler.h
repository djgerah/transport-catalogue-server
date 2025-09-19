#pragma once

#include <optional>
#include <sstream>

#include "json.h"
#include "map_renderer.h"
#include "transport_catalogue.h"
#include "transport_router.h"

class RequestHandler
{
  public:
    RequestHandler(const tc::TransportCatalogue &catalogue, const renderer::MapRenderer &renderer,
                   const tc::TransportRouter &router)
        : catalogue_(catalogue), renderer_(renderer), router_(router)
    {
    }

    const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;
    const std::optional<graph::Router<double>::RouteInfo> GetRoute(const tc::Stop *stop_from,
                                                                   const tc::Stop *stop_to) const;
    const graph::DirectedWeightedGraph<double> &GetGraph() const;
    svg::Document RenderMap() const;

  private:
    const tc::TransportCatalogue &catalogue_;
    const renderer::MapRenderer &renderer_;
    const tc::TransportRouter &router_;
};