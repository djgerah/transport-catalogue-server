#pragma once

#include "json.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "transport_catalogue.h"

namespace json_reader
{
struct CommandDescription
{

    explicit operator bool() const
    {
        return !command.empty();
    }

    bool operator!() const
    {
        return !operator bool();
    }

    std::string command;
    std::string id;
    json::Dict description;
};

class JsonReader
{
  public:
    JsonReader(std::istream &document) : document_(json::Load(document))
    {
    }

    const json::Node &GetBaseRequests() const;
    const json::Node &GetStatRequests() const;
    const json::Node &GetRenderSettings() const;
    const json::Node &GetRoutingSettings() const;
    const json::Node PrintBus(const json::Dict &request, tc::TransportCatalogue &catalogue_) const;
    const json::Node PrintStop(const json::Dict &request, tc::TransportCatalogue &catalogue_,
                               RequestHandler &request_handler) const;
    const json::Node PrintMap(const json::Dict &request, RequestHandler &request_handler) const;
    const json::Node PrintRoute(const json::Dict &request, tc::TransportCatalogue &catalogue_,
                                RequestHandler &request_handler) const;
    void ProcessRequests(const json::Node &stat_requests, tc::TransportCatalogue &catalogue,
                         RequestHandler &request_handler, std::ostream &output) const;
    void FillTransportCatalogue(tc::TransportCatalogue &catalogue);
    renderer::RenderSettings FillRenderSettings(const json::Node &settings) const;
    tc::RoutingSettings FillRoutingSettings(const json::Node &settings) const;
    void ApplyCommands(tc::TransportCatalogue &catalogue) const;
    void ParseRequest(const json::Node &request);

  private:
    tc::Stop MakeStop(const json_reader::CommandDescription &c) const;
    tc::Bus MakeBus(const json_reader::CommandDescription &c, tc::TransportCatalogue &catalogue) const;
    void ProcessColors(const json::Dict &request, renderer::RenderSettings &render_settings) const;
    svg::Rgb MakeRGB(const json::Array &type) const;
    svg::Rgba MakeRGBA(const json::Array &type) const;
    void AddDistance(const json_reader::CommandDescription &c, tc::TransportCatalogue &catalogue) const;
    static CommandDescription ParseCommandDescription(const json::Node &request);
    static std::vector<const tc::Stop *> ParseRoute(const json::Dict &description, tc::TransportCatalogue &catalogue);

    json::Document document_;
    std::vector<CommandDescription> commands_;
};
} // end namespace json_reader